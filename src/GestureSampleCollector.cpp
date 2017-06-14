#include "GestureSampleCollector.hpp"

GestureSampleCollector::GestureSampleCollector(HandDetector *hand_detector,
                                               SampleCollector *sample_collector,
                                               QObject *parent) :
    QObject(parent),
    main_view(MainView::getInstance()),
    settings_view(SettingsView::getInstance()),
    monitor_view(MonitorView::getInstance()),
    work_status(_work_status),
    camera_fps(_camera_fps),
    roi(_roi),
    _settings(Settings::getInstance()),
    _work_status(STATUS_IDLE),
    _camera_fps(CAMERA_FPS),
    _camera(new cv::VideoCapture),
    _camera_capture_timer(new QTimer),
    _hand_detector(hand_detector),
    _sample_collector(sample_collector)
{
    connect(main_view, SIGNAL(mainViewClosing()), this, SLOT(windowClosing()));
    connect(main_view, SIGNAL(cameraRequest()), this, SLOT(openCamera()));
    connect(main_view, SIGNAL(samplingTaskStartRequest(int,QString)), this, SLOT(startSamplingTask(int,QString)));
    connect(main_view, SIGNAL(samplingTaskStopRequest()), this, SLOT(stopSamplingTask()));
    connect(main_view, SIGNAL(monitorWindowRequest()), this, SLOT(openMonitorWindow()));
    connect(main_view, SIGNAL(settingsWindowRequest()), this, SLOT(openSettingsWindow()));
    connect(main_view, SIGNAL(settingsWindowGestureTabRequest()), this, SLOT(openSettingsWindowGestureTab()));

    connect(settings_view, SIGNAL(changeSkinColorLowerBound(int,int,int)), _hand_detector, SLOT(setSkinColorFilterLowerBound(int,int,int)));
    connect(settings_view, SIGNAL(changeSkinColorUpperBound(int,int,int)), _hand_detector, SLOT(setSkinColorFilterUpperBound(int,int,int)));
    connect(settings_view, SIGNAL(changeDetectionArea(int)), _hand_detector, SLOT(setDetectionArea(int)));
    connect(settings_view, SIGNAL(changeMorphology(bool)), _hand_detector, SLOT(setMorphology(bool)));
    connect(settings_view, SIGNAL(backgroundSettingRequest()), _hand_detector, SLOT(setBackgroundImage()));
    connect(settings_view, SIGNAL(backgroundClearingRequest()), _hand_detector, SLOT(clearBackgroundImage()));
    connect(settings_view, SIGNAL(changeRoiRange(int,int,int,int)), this, SLOT(verifyRoiRange(int,int,int,int)));
    connect(settings_view, SIGNAL(changeLabelList()), main_view, SLOT(reloadLabelList()));

    connect(_hand_detector, SIGNAL(backgroundImageSet()), this, SLOT(updateBackgroundImage()));
    connect(_hand_detector, SIGNAL(backgroundImageCleared()), settings_view, SLOT(clearBackgroundImage()));

    connect(this, SIGNAL(cameraOpened()), main_view, SLOT(cameraStarted()));
    connect(this, SIGNAL(cameraOpened()), settings_view, SLOT(enableBackgroundSetting()));
    connect(this, SIGNAL(cameraReleased()), main_view, SLOT(cameraReleased()));
    connect(this, SIGNAL(cameraReleased()), this, SLOT(_changeWorkStatusToNothing()));
    connect(this, SIGNAL(cameraReleased()), settings_view, SLOT(disableBackgroundSetting()));
    connect(this, SIGNAL(samplingTaskStarted()), main_view, SLOT(samplingTaskStarted()));
    connect(this, SIGNAL(samplingTaskStopped()), main_view, SLOT(samplingTaskStopped()));
    connect(this, SIGNAL(samplingTaskStopped()), this, SLOT(_changeWorkStatusToNothing()));
    
    connect(_camera_capture_timer, SIGNAL(timeout()), this, SLOT(captureFrame()));

    settings_view->setToCurrentSettings();
}

GestureSampleCollector::~GestureSampleCollector()
{
    delete _camera_capture_timer;
    delete _camera;
    delete _hand_detector;
    delete _sample_collector;
}

void GestureSampleCollector::run()
{
    main_view->show();
}

void GestureSampleCollector::openSettingsWindow()
{
    settings_view->activatePage(SettingsView::PAGE_GENERAL);
    settings_view->show();
    settings_view->raise();
}

void GestureSampleCollector::openSettingsWindowGestureTab()
{
    settings_view->activatePage(SettingsView::PAGE_GESTURE_LIST);
    settings_view->show();
    settings_view->raise();
}

void GestureSampleCollector::openMonitorWindow()
{
    monitor_view->show();
    monitor_view->raise();
}

void GestureSampleCollector::verifyRoiRange(const int &start_x, const int &end_x, const int &start_y, const int &end_y)
{
    _roi.x = 307;
    _roi.y = 9;
    _roi.width = 320;
    _roi.height = 320;
    // _roi.x = main_view->getVideoFrameWidth() * start_x/100;
    // _roi.y = main_view->getVideoFrameHeight() * start_y/100;
    // _roi.width = main_view->getVideoFrameWidth() * (end_x-start_x)/100;
    // _roi.height = main_view->getVideoFrameHeight() * (end_y-start_y)/100;

    // if (_roi.width < DEFAULT_ROI_MARGIN_LEFT + DEFAULT_ROI_MARGIN_RIGHT + 1)
    // {
    //     _roi.width = DEFAULT_ROI_MARGIN_LEFT + DEFAULT_ROI_MARGIN_RIGHT + 1;
    //     if (_roi.x + _roi.width > main_view->getVideoFrameWidth())
    //         _roi.x = main_view->getVideoFrameWidth() - _roi.width;
    // }
    // if (_roi.height < DEFAULT_ROI_MARGIN_TOP + DEFAULT_ROI_MARGIN_BOTTOM + 1)
    // {
    //     _roi.height = DEFAULT_ROI_MARGIN_TOP + DEFAULT_ROI_MARGIN_BOTTOM + 1;
    //     if (_roi.y + _roi.height > main_view->getVideoFrameHeight())
    //         _roi.y = main_view->getVideoFrameHeight() - _roi.height;
    // }
}

void GestureSampleCollector::updateBackgroundImage()
{
    settings_view->setBackgroundImage(
                CvQtImgConvertor::cvMat2QPixmap(
                    this->_hand_detector->background_img
                    )
                );
}

void GestureSampleCollector::openCamera()
{
    if (_camera->isOpened())
    {
        _camera->open(0);
        if (!_camera->isOpened())
        {
            _handleCameraError();
            return;
        }
    }
    emit cameraOpened();
    _camera_capture_timer->start(1000/_camera_fps);
}

void GestureSampleCollector::releaseCamera()
{
    _camera_capture_timer->stop();
    // FIXME exception caused by opencv when releasing the camera
    //    if (_camera->isOpened())
    //        _camera->release();
}

void GestureSampleCollector::captureFrame()
{
    if (_camera->isOpened())
    {
        cv::Mat captured_frame;
        _camera->read(captured_frame);
        if (!captured_frame.empty())
        {
            // resize and keep aspect ratio
            cv::resize(captured_frame, captured_frame,
                       cv::Size(captured_frame.cols*main_view->getVideoFrameHeight()/captured_frame.rows,
                                main_view->getVideoFrameHeight()));
            captured_frame(
                        cv::Rect(
                            (captured_frame.cols - main_view->getVideoFrameWidth())/2, 0,
                            main_view->getVideoFrameWidth(), main_view->getVideoFrameHeight()
                            )
                        ).copyTo(captured_frame);
            cv::flip(captured_frame, captured_frame, 1);
            _processCapturedFrame(captured_frame);
            return;
        }
    }
    _handleCameraError();
}

void GestureSampleCollector::startSamplingTask(const int &label_index, const QString &folder_path)
{
    if (_work_status == STATUS_SAMPLING)
        return;
    if (!_camera->isOpened())
    {
        _handleCameraError();
        return;
    }
    if (label_index < 0 || label_index >= _settings->gesture_list.size())
    {
        _handleSamplingError(SAMPLING_ERROR_GESTURE_INDEX);
        return;
    }
    
    if (_sample_collector->setStoragePath(folder_path, _settings->gesture_list.at(label_index)))
    {
        if (QMessageBox::Ok == QMessageBox::information(
                    main_view, tr("Start Sampling"),
                    QString(tr("<div style=\"font-weight:100\">"
                               "Sampling will start.<br />"
                               "It will automatically terminate after %1 of samples are collected and take about %2 seconds. "
                               "All samples collected will be stored to"
                               "<pre style=\"padding-left:10px\">%3</pre>"
                               "Please pose your gesture in the region of interesting."
                               "</div>")
                            ).arg(QString::number(_settings->sampling_amount_per_time),
                                  QString::number(3*_settings->sampling_amount_per_time*_settings->sampling_interval/1000),
                                  _sample_collector->storage_path.toHtmlEscaped()),
                    QMessageBox::Cancel, QMessageBox::Ok))
        {
            _sampling_trails = 0;
            _samples_collected = 0;
            _settings->setSampleStoragePath(folder_path);
            _settings->setSelectedGesture(label_index);
            emit samplingTaskStarted();
            _work_status = STATUS_SAMPLING;
        }
        else
            emit samplingTaskStopped();
        return;
    }

    _handleSamplingError(SAMPLING_ERROR_STORAGE_PATH);
}

void GestureSampleCollector::stopSamplingTask()
{
    _samplingCompleted();
}

void GestureSampleCollector::windowClosing()
{
    monitor_view->close();
    settings_view->close();
    releaseCamera();
}

void GestureSampleCollector::_changeWorkStatusToNothing()
{
    _work_status = STATUS_IDLE;
}

void GestureSampleCollector::_handleCameraError()
{
    _camera_capture_timer->stop();
    emit cameraReleased();
    QMessageBox::critical(main_view, tr("Error"), tr("Failed to open camera."));
}

void GestureSampleCollector::_processCapturedFrame(cv::Mat &captured_frame)
{
    if (monitor_view->isVisible() || _hand_detector->waitting_bg || _work_status == STATUS_SAMPLING)
    {
        bool detected = _hand_detector->detect(captured_frame(_roi));

        if (_work_status == STATUS_SAMPLING && !_sample_collector->deny())
        {
            if (detected)
                _sample(_hand_detector->interesting_img, _hand_detector->extracted_img);
            else
                main_view->appendText(tr("[Error] Sampling failed. Nothing detected."));
        }
           

        if (monitor_view->isVisible())
        {
            if (_hand_detector->extracted_img.empty())
                monitor_view->updateMonitorImage3(
                            CvQtImgConvertor::cvMat2QPixmap(_hand_detector->interesting_img),
                            CvQtImgConvertor::cvMat2QPixmap(_hand_detector->filtered_img),
                            CvQtImgConvertor::cvMat2QPixmap(_hand_detector->convexity_img)
                            );
            else
                monitor_view->updateMonitorImage4(
                            CvQtImgConvertor::cvMat2QPixmap(_hand_detector->interesting_img),
                            CvQtImgConvertor::cvMat2QPixmap(_hand_detector->filtered_img),
                            CvQtImgConvertor::cvMat2QPixmap(_hand_detector->convexity_img),
                            CvQtImgConvertor::cvMat2QPixmap(_hand_detector->extracted_img)
                            );
        }
    }
    
    cv::rectangle(captured_frame, _roi, HandDetector::COLOR_GREEN, 2);
    main_view->updateVideoFrame(CvQtImgConvertor::cvMat2QPixmap(captured_frame));

}

void GestureSampleCollector::_sample(const cv::Mat &orig_img, const cv::Mat &proc_img)
{
    if (_sample_collector->sample(orig_img, proc_img))
    {
        _samples_collected++;
        main_view->updateText(QString(tr("[Info] Collected: %1 / %2")).arg(
                                      QString::number(++_sampling_trails).rightJustified(3, ' '),
                                      QString::number(_settings->sampling_amount_per_time))
                                  );
        if (_sampling_trails >= _settings->sampling_amount_per_time)
            _samplingCompleted();
    }
    else
        _handleSamplingError(SAMPLING_ERROR_STORAGE_IMAGE);
}

void GestureSampleCollector::_handleSamplingError(const SAMPLING_ERROR &e)
{
    QString msg;
    if (e == SAMPLING_ERROR_GESTURE_INDEX)
    {
        msg = QString(tr("Illegal Gesture Information."));
    }
    else
        msg = QString(tr("Failed to store sample images at <pre style=\"padding-left:10px\">%1</pre>")).arg(
                    _sample_collector->storage_path.toHtmlEscaped());
    QMessageBox::critical(main_view, tr("Error"),
                          QString(tr("<div style=\"font-weight:100\">"
                                     "Sampling terminated.<br /><br />"
                                     "%1"
                                     "</div>")).arg(msg)
                          );
    emit samplingTaskStopped();
}

void GestureSampleCollector::_samplingCompleted()
{
    emit samplingTaskStopped();
    main_view->appendText(QString(tr("[Info] Sampling Completed.\n"
                                         "[Info] %1 Samples were stored at\n%2\n"
                                         )).arg(
                                  QString::number(_samples_collected),
                                  _sample_collector->storage_path.toHtmlEscaped()
                                  )
                              );
    QMessageBox::information(main_view, tr("Sampling Completed"),
                             QString(tr("<div style=\"font-weight:100\">%1 samples were collected. All samples were stored at<pre style=\"padding-left:10px\">%3</pre></div>")).arg(
                                 QString::number(_samples_collected),
                                 _sample_collector->storage_path.toHtmlEscaped()
                                 )
                             );
}