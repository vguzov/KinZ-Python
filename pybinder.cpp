#include <pybind11/pybind11.h>
#include <pybind11/stl.h>
#include <k4a/k4a.h>
#include <string>
#include "kinect.h"

namespace py = pybind11;

PYBIND11_MODULE(kinz, m) {

    py::class_<BufferDepth>(m, "BufferDepth", py::buffer_protocol())
   .def_buffer([](BufferDepth &m) -> py::buffer_info {
        return py::buffer_info(
            m.data(),
            sizeof(uint16_t),
            py::format_descriptor<uint16_t>::format(),
            2,
            { m.rows(), m.cols()},
            { m.stride(), sizeof(uint16_t) }
        );
    });

    py::class_<BufferColor>(m, "BufferColor", py::buffer_protocol())
   .def_buffer([](BufferColor &m) -> py::buffer_info {
        return py::buffer_info(
            m.data(),
            sizeof(uint8_t),
            py::format_descriptor<uint8_t>::format(),
            3,
            { m.rows(), m.cols(), size_t(4)},
            { sizeof(unsigned char) * 4 * m.cols(),
              sizeof(unsigned char) * 4,
              sizeof(unsigned char) }
        );
    });

    py::class_<BufferBodyIndex>(m, "BufferBodyIndex", py::buffer_protocol())
   .def_buffer([](BufferBodyIndex &m) -> py::buffer_info {
        return py::buffer_info(
            m.data(),
            sizeof(uint8_t),
            py::format_descriptor<uint8_t>::format(),
            2,
            { m.rows(), m.cols()},
            { m.stride(), sizeof(uint8_t) }
        );
    });

    py::class_<BufferPointCloud>(m, "BufferPointCloud", py::buffer_protocol())
   .def_buffer([](BufferPointCloud &m) -> py::buffer_info {
        return py::buffer_info(
            m.data(),
            sizeof(int16_t),
            py::format_descriptor<int16_t>::format(),
            3,
            { m.rows(), m.cols(), size_t(3)},
            { m.stride(),
              sizeof(int16_t) * 3,
              sizeof(int16_t)
            }
        );
    });


    py::class_<Calibration>(m, "Calibration")
        .def(py::init())
        .def("get_size", &Calibration::get_size)
        .def("get_intrinsics_matrix", &Calibration::get_intrinsics_matrix,
            py::arg("extended"))
        .def("get_distortion_params", &Calibration::get_distortion_params)
        .def("get_rotation_matrix", &Calibration::get_rotation_matrix)
        .def("get_translation_vector", &Calibration::get_translation_vector)
        .def("get_camera_pose", &Calibration::get_camera_pose)
        .def_readonly("width", &Calibration::width);


    py::class_<Imu_sample>(m, "Imu_sample")
        .def(py::init())
        .def_readonly("temperature", &Imu_sample::temperature)
        .def_readonly("acc_x", &Imu_sample::acc_x)
        .def_readonly("acc_y", &Imu_sample::acc_y)
        .def_readonly("acc_z", &Imu_sample::acc_z)
        .def_readonly("acc_timestamp_usec", &Imu_sample::acc_timestamp_usec)
        .def_readonly("gyro_x", &Imu_sample::gyro_x)
        .def_readonly("gyro_y", &Imu_sample::gyro_y)
        .def_readonly("gyro_z", &Imu_sample::gyro_z)
        .def_readonly("gyro_timestamp_usec", &Imu_sample::gyro_timestamp_usec);


    py::class_<ColorData>(m, "ColorData")
        .def(py::init())
        .def_readonly("buffer", &ColorData::buffer)
        .def_readonly("system_timestamp_nsec", &ColorData::system_timestamp_nsec)
        .def_readonly("device_timestamp_usec", &ColorData::device_timestamp_usec);


    py::class_<DepthData>(m, "DepthData")
        .def(py::init())
        .def_readonly("buffer", &DepthData::buffer)
        .def_readonly("system_timestamp_nsec", &DepthData::system_timestamp_nsec)
        .def_readonly("device_timestamp_usec", &DepthData::device_timestamp_usec);

    py::class_<BodyIndexData>(m, "BodyIndexData")
        .def(py::init())
        .def_readonly("buffer", &BodyIndexData::buffer)
        .def_readonly("system_timestamp_nsec", &BodyIndexData::system_timestamp_nsec)
        .def_readonly("device_timestamp_usec", &BodyIndexData::device_timestamp_usec);


    py::class_<Kinect>(m, "Kinect")
        .def(py::init<uint8_t, int, bool, bool, uint8_t, bool, bool, bool, bool, bool, bool, const std::string, int>(),
             py::arg("deviceIndex")=0, py::arg("resolution")=1080, py::arg("wfov")=false,
             py::arg("binned")=true, py::arg("framerate")=30, py::arg("sensor_color")=true,
             py::arg("sensor_depth")=true, py::arg("sensor_ir")=true,
             py::arg("imu_sensors")=false, py::arg("body_tracking")=false,
             py::arg("body_index")=false, py::arg("sync_mode")=std::string("none"),
             py::arg("sync_capture_delay")=0)
        .def("get_frames", &Kinect::get_frames, "Read frames from Kinect",
            py::arg("get_color")=true, py::arg("get_depth")=true,
            py::arg("get_ir")=true, py::arg("get_sensors")=false,
            py::arg("get_body")=false, py::arg("get_body_index")=false,
            py::arg("align_depth")=false)
        .def("get_sensor_data", &Kinect::get_sensor_data, "Return sensor struct")
        .def("get_color_data", &Kinect::get_color_data, "Return color frame")
        .def("get_depth_data", &Kinect::get_depth_data, "Return depth frame")
        .def("get_ir_data", &Kinect::get_ir_data, "Return IR frame")
        .def("get_pointcloud", &Kinect::get_pointcloud, "Return point cloud")
        .def("get_pointcloud_color", &Kinect::get_pointcloud_color, "Return point cloud color values")
        .def("save_pointcloud", &Kinect::save_pointcloud, "Save the current pointcloud to a ply file",
            py::arg("file_name"))
        .def("get_depth_calibration", &Kinect::get_depth_calibration, py::return_value_policy::copy)
        .def("get_color_calibration", &Kinect::get_color_calibration, py::return_value_policy::copy)
        .def("get_raw_calibration", &Kinect::get_raw_calibration, "Return the raw calibration string")
        .def("get_serial_number", &Kinect::get_serial_number, "Return the serial number of the kinect")
        .def("close", &Kinect::close)
        .def("get_camera_activation_status", &Kinect::get_camera_activation_status)
        .def("start_cameras", &Kinect::start_cameras)
        .def("stop_cameras", &Kinect::stop_cameras)
        .def("get_last_frameget_timestamp_usec", &Kinect::get_last_frameget_timestamp_us)
        .def("set_exposure", &Kinect::set_exposure, "Set exposure time",
            py::arg("exposure"))
        .def("set_gain", &Kinect::set_gain, "Set sensor gain",
            py::arg("gain"))
        .def("get_exposure", &Kinect::get_exposure, "Get exposure time")
        .def("map_coords_color_to_depth", &Kinect::map_coords_color_to_depth,
            "Map color pixel coordinates to depth image space")
        .def("map_coords_color_to_3D", &Kinect::map_coords_color_to_3D,
            "Map color pixel coordinates to 3D space of depth camera",
            py::arg("color_coords"), py::arg("depth_reference")=true)
        .def("map_coords_depth_to_color", &Kinect::map_coords_depth_to_color,
            "Map depth pixel coordinates to color image space")
        .def("map_coords_depth_to_3D", &Kinect::map_coords_depth_to_3D,
            "Map depth pixel coordinates to 3D space of depth camera",
            py::arg("depth_coords"), py::arg("depth_reference")=true)
        .def("map_coords_3d_to_depth", &Kinect::map_coords_3d_to_depth,
            "Map 3D coordinates to depth space",
            py::arg("coords3d"), py::arg("depth_reference")=true)
        .def("map_coords_3d_to_color", &Kinect::map_coords_3d_to_color,
            "Map 3D coordinates to color space",
            py::arg("coords3d"), py::arg("depth_reference")=true)
        .def("get_depth2pc_map", &Kinect::get_image2pc_map<K4A_CALIBRATION_TYPE_DEPTH, K4A_CALIBRATION_TYPE_DEPTH>,
             "Generate a map of xy offsets for depth to pointcloud transformation")
        .def("get_color2pc_map", &Kinect::get_image2pc_map<K4A_CALIBRATION_TYPE_COLOR, K4A_CALIBRATION_TYPE_COLOR>,
             "Generate a map of xy offsets for color to pointcloud transformation")
        .def("get_depth2color_rotation_matrix", &Kinect::get_cameras_rotation_matrix<K4A_CALIBRATION_TYPE_DEPTH, K4A_CALIBRATION_TYPE_COLOR>,
             "Returns the rotation to transform from depth to color sensors coordinate frames")
        .def("get_depth2color_translation_vector", &Kinect::get_cameras_translation_vector<K4A_CALIBRATION_TYPE_DEPTH, K4A_CALIBRATION_TYPE_COLOR>,
             "Returns the translation to transform from depth to color sensors coordinate frames")
        .def("get_color2depth_rotation_matrix", &Kinect::get_cameras_rotation_matrix<K4A_CALIBRATION_TYPE_COLOR, K4A_CALIBRATION_TYPE_DEPTH>,
             "Returns the rotation to transform from color to depth sensors coordinate frames")
        .def("get_color2depth_translation_vector", &Kinect::get_cameras_translation_vector<K4A_CALIBRATION_TYPE_COLOR, K4A_CALIBRATION_TYPE_DEPTH>,
             "Returns the translation to transform from color to depth sensors coordinate frames")
        #ifdef BODY
        .def("get_num_bodies", &Kinect::get_num_bodies, "Get number of bodies found")
        .def("get_bodies", &Kinect::get_bodies, "Get bodies list")
        .def("get_body_index_map", &Kinect::get_body_index_map, "Return body index map frame",
            py::arg("returnId")=false, py::arg("inColor")=false)
        #endif
        ;
}