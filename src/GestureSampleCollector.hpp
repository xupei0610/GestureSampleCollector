#ifndef GESTURESAMPLECOLLECTOR_H
#define GESTURESAMPLECOLLECTOR_H
/**
 * @file
 * @author Pei Xu, xupei0610 at gmail.com
 * @brief The GestureSampleCollector.h file contains the main class of the sample collector.
 */
#include <QObject>
#include <QTimer>
#include <QString>

#include "Settings.hpp"
#include "MainView.hpp"
#include "SettingsView.hpp"
#include "MonitorView.hpp"
#include "HandDetector.hpp"
#include "SampleCollector.hpp"

/**
 * @brief The GestureSampleCollector class is the main class of the sample collector.
 */
class GestureSampleCollector final : public QObject
{
    Q_OBJECT
public:
    /**
     * @brief WORK_STATUS represents the current work status of the system.
     */
    enum WORK_STATUS
    {
        STATUS_IDLE,         //!< idle
        STATUS_SAMPLING,     //!< sampling
    };
    /**
     * @brief SAMPLING_ERROR represents the error appearing during samples.
     *
     * @see #GestureControlSystem::startSamplingTask
     * @see #GestureControlSystem::_handleSamplingError
     */
    enum SAMPLING_ERROR
    {
        SAMPLING_ERROR_GESTURE_INDEX, //!< illegal gesture index
        SAMPLING_ERROR_STORAGE_PATH,  //!< illegal storage path or no permissions to make directory at the given path
        SAMPLING_ERROR_STORAGE_IMAGE  //!< failed to store sample images at the given path
    };
    /**
     * @brief main_view is the GUI of the main window.
     *
     * @see #MainView
     */
    MainView *main_view;
    /**
     * @brief settings_view is the GUI of the setting window.
     *
     * @see #SettingsView
     */
    SettingsView *settings_view;
    /**
     * @brief monitor_view is the GUI of the monitor window.
     *
     * @see #MonitorView
     */
    MonitorView *monitor_view;
    /**
     * @brief work_status is the current work status. This is a reference to #GestureControlSystem::_work_status.
     *
     * @see #GestureControlSystem::WORK_STATUS
     */
    const WORK_STATUS &work_status;
    /**
     * @brief camera_fps is the FPS of the camera.
     */
    const unsigned int &camera_fps;
    /**
     * @brief _roi is the region of interesting on the frame captured by the camera.
     */
    const cv::Rect &roi;
    /**
     * @brief GestureSampleCollector is the constructor of the sample collector class.
     * @param hand_detector : a hand detector class
     * @param sample_collector : a sample collector class
     * @param parent : a useless parameter here
     */
    GestureSampleCollector(HandDetector *hand_detector,
                           SampleCollector *sample_collector, QObject *parent=0);
    ~GestureSampleCollector();
    /**
     * @brief run is the main function to run the system
     */
    void run();
signals:
    /**
     * @brief cameraOpened is the signal emitted right after the camera launched.
     */
    void cameraOpened();
    /**
     * @brief cameraReleased is the signal emitted right after the camera is released.
     */
    void cameraReleased();
    /**
     * @brief controllingTaskStarted is the signal to indicate the start of sampling task.
     */
    void samplingTaskStarted();
    /**
     * @brief controllingTaskStarted is the signal to indicate the stop of sampling task.
     */
    void samplingTaskStopped();

public slots:
    /**
     * @brief openSettingWindow opens the setting window.
     */
    void openSettingsWindow();
    /**
     * @brief openSettingWindowGestureTab opens the gesture list page of the setting window
     */
    void openSettingsWindowGestureTab();
    /**
     * @brief openMonitorWindow opens the monitor window.
     */
    void openMonitorWindow();
    /**
     * @brief verifyRoiRange verifies the given ROI range and stores the verified range.
     *
     * @param start_x : the left bound of the ROI in percent
     * @param end_x : the right bound of the ROI in percent
     * @param start_y : the top bound of the ROI in percent
     * @param end_y : the bottom bound of the ROI in percent
     */
    void verifyRoiRange(const int &start_x, const int &end_x, const int &start_y, const int &end_y);
    /**
     * @brief updateBackgroundImage updates the background image shown in the setting window.
     */
    void updateBackgroundImage();
        /**
     * @brief openCamera opens the camera and shows error message if the camera cannot be open.
     * @see #GestureControlSystem::_handleCameraError
     */
    void openCamera();
    /**
     * @brief releaseCamera closes the camera.
     *
     * **ATTENTION**:
     *   This function cannot work properly on Mac OS (at least at Mac OS 10.12.3) due to the exception caused by opencv when releasing the camera on MAC OS
     */
    void releaseCamera();
    /**
     * @brief captureFrame captures frame from the camera and analyzes the captured frame. It will show an error message if it failed to capture frame from the camera.
     *
     * @see #GestureControlSystem::_processCapturedFrame
     * @see #GestureControlSystem::_handleCameraError
     */
    void captureFrame();
    /**
     * @brief startSamplingTask starts a sampling task.
     *
     * It sets #GestureControlSystem::_work_status and emits the signal of #GestureControlSystem::samplingTaskStarted if everything goes will, or shows error message if something wrong.
     * It should reset #GestureControlSystem::_sampling_trails and #GestureControlSystem::_samples_collected to 0.
     *
     * @param label_index : index of the label into which samples will be added
     * @param folder_path : path of the folder containing the sample
     *
     */
    void startSamplingTask(const int &label_index, const QString &folder_path);
    /**
     * @brief stopSamplingTask handles with the request of stopping sampling.
     *
     * It, like #GestureControlSystem::_samplingCompleted, resets #GestureControlSystem::_work_status and emit the signal of #GestureControlSystem::controllingTaskStopped .
     *
     * @see #GestureControlSystem::_samplingCompleted
     */
    void stopSamplingTask();
    /**
     * @brief windowClosing is the callback function before the main window is closed.
     */
    void windowClosing();
private:
    Settings *_settings;
    WORK_STATUS _work_status;
    cv::Rect _roi;
    int unsigned _camera_fps;
    cv::VideoCapture *_camera;
    QTimer *_camera_capture_timer;
    HandDetector *_hand_detector;
    SampleCollector *_sample_collector;
    /**
     * _sampling_trails is an indicator of how many sampling trails have been conducted.
     *
     * **ATTENTION**:
     *  It should be reset to 0 everytime before doing a batch of sampling.
     */
    int _sampling_trails;
    /**
     * _samples_collected is an indicator of how many samples have benn collected.
     *
     * **ATTENTION**:
     *  It should be reset to 0 everytime before doing a batch of sampling.
     */
    int _samples_collected;
    /**
     * _handleCameraError is the callback function to handle the camera error.
     *
     * It do the following by default:
     *  - stop #GestureControlSystem::_camera_capture_timer
     *  - emit #GestureControlSystem::cameraReleased() to inform the release of the camera.
     */
    void _handleCameraError();
    /**
     * _processCapturedFrame is the callback function to deal with the frame captured by the camera.
     *
     * It do the following by default:
     *
     *  - detect the hand/gesture via #HandDetector from the region of interesting on the captured frame,
     *  - make command via #CommandMaker using the analyst result obtained by #GestureAnalyst, and
     *  - draw the region of interesting on the captured frame and show the captured frame on the tracking window.
     *
     */
    void _processCapturedFrame(cv::Mat &captured_frame);
    /**
     * _sample does sampling via #SampleCollector::sample
     */
    void _sample(const cv::Mat &orig_img, const cv::Mat &proc_img);
    /**
     * _sampling_completed is the callback function when sampling completed.
     *
     * It informs the success of sampling and emits the signal of #GestureControlSystem::samplingTaskStopped
     */
    void _samplingCompleted();
    /**
     * _handleSamplingError is the callback function when sampling failed.
     *
     * It informs the failing of sampling and emits the signal of #GestureControlSystem::samplingTaskStopped
     */
    void _handleSamplingError(const SAMPLING_ERROR &e);
private slots:
    /*
     * _changeWorkStatusToNothing sets the work status to STATUS_IDLE.
     */
    void _changeWorkStatusToNothing();
    
};

#endif