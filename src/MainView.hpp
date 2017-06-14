#ifndef MAINVIEW_H
#define MAINVIEW_H
/**
 * @file
 * @author Pei Xu, xupei0610 at gamil.com
 * @brief The MainView.hpp file contains the class #MainView that provides the GUI of the main window.
 */
#include <QWidget>
#include <QLabel>
#include <QLineEdit>
#include <QComboBox>
#include <QPushButton>
#include <QPlainTextEdit>
#include <QGridLayout>
#include <QSizePolicy>
#include <QFileDialog>
#include <QTextCursor>
#include <QFileInfo>
#include <QString>
#include <QCloseEvent>

#include "Singleton.hpp"
#include "Settings.hpp"
/**
 * @brief The MainView class provides the GUI of the main window.
 * 
 * This is a singleton class. Use #MainView::getInstance() to get the instance of this class.
 */
class MainView final : public QDialog
{
    Q_OBJECT
public:
    /**
     * @brief WORK_STATUS represents the current work status.
     */
    enum WORK_STATUS
    {
        STATUS_WAITING_CAMERA, //!< camera does not starts
        STATUS_IDLE,           //!< work does not starts but camera starts
        STATUS_WORKING,        //!< work is in progress
    };

    /**
     * @brief work_status is current work status.
     */
    WORK_STATUS &work_status;

    /**
     * @brief getInstance returns the singleton instance of this class.
     * @return the singleton instance of #MainView class.
     *
     * @see #Singleton
     */
    static MainView *getInstance();

    /**
     * @brief updateVideoFrame displays the given image on the video label widget.
     *
     * The given image will be sacled according to the size of the widget to display the video.
     *
     * @param img : image of the current video frame
     */
    void updateVideoFrame(const QPixmap &img);
    /**
     * @brief getVideoFrameWidth returns the width of the label showing the video frame.
     * @return the width, in pixel, of the label showing the video frame
     * @see #MainView::getVideoFrameHeight
     */
    int getVideoFrameWidth();
    /**
     * @brief getVideoFrameHeight returns the height of the label showing the video frame.
     * @return the height, in pixel, of the label showing the video frame
     * @see #MainView::getVideoFrameWidth
     */
    int getVideoFrameHeight();
    /**
     * @brief appendText adds text on the text panel in a new line.
     * @param text : text that will be added onto the text panel
     *
     * @see #MainView::updateText
     * @see #MainView::clearText
     * @see #MainView::setText
     */
    void appendText(const QString &text);
    /**
     * @brief updateText updates the content of the last line of the text panel.
     * @param new_text : new text that will be shown on the last line of the text panel
     *
     * @see #MainView::appendText
     * @see #MainView::clearText
     * @see #MainView::setText
     */
    void updateText(const QString &new_text);
    /**
     * @brief clearText clears the text panel.
     *
     * @see #MainView::appendText
     * @see #MainView::updateText
     * @see #MainView::setText
     */
    void clearText();
    /**
     * @brief setText sets the content of the text panel.
     *
     * It combines #MainView::clearText and #MainView::appendText together but should be more efficient perhaps.
     *
     * @param text : text that will be shown on the text panel
     * @see #MainView::clearText
     */
    void setText(const QString &text);

signals:
    /**
     * @brief mainViewClosing is the signal that indicates the main GUI is closing.
     */
    void mainViewClosing();
    /**
     * @brief cameraRequest is the signal of the request for launching the camera.
     */
    void cameraRequest();
    /**
     * @brief samplingTaskStartRequest is the signal of the request for starting sampling task.
     * @param label_index : index of the label into which samples will be added
     * @param folder_path : path of the folder containing the sample
     *
     * @see #MainView::samplingStarted
     * @see #MainView::samplingTaskStopRequest
     */
    void samplingTaskStartRequest(const int &label_index, const QString &folder_path);
    /**
     * @brief samplingTaskStopRequest is the signal of the request for stopping sampling task.
     *
     * @see #MainView::samplingStopped
     * @see #MainView::samplingTaskStartRequest
     */
    void samplingTaskStopRequest();
    /**
     * @brief monitorWindowRequest is the signal of the request for the monitor window.
     */
    void monitorWindowRequest();
    /**
     * @brief settingWindowRequest is the signal of the request for the setting window.
     */
    void settingsWindowRequest();
    /**
     * @brief settingsWindowGestureTabRequest is the signal of the request for the gesture list page of the setting window.
     */
    void settingsWindowGestureTabRequest();

public slots:
    /**
     * @brief reloadLabelList is the callback after the gesture label list is changed
     *
     * It updates the gesture label list using data from #Settings::gesture_list .
     * @see #SettingView::changeLabelList
     */
    void reloadLabelList();
    /**
     * @brief cameraStarted is the callback when the camera is started.
     *
     * It updates the text on the start button to `control` or `sample`.
     *
     * @see #MainView::cameraRequest
     */
    void cameraStarted();
    /**
     * @brief cameraReleased is the callback when the camera is closed.
     *
     * It sets the work status to waiting for camera and update UI.
     *
     * @see #MainView::controllingTaskStopped
     * @see #MainView::samplingTaskStopped
     */
    void cameraReleased();
    /**
     * @brief samplingTaskStarted is the callback after sampling task starts.
     *
     * It changes the work status and update UI.
     *
     * @see #MainView::samplingTaskStartRequest
     * @see #MainView::controllingTaskStarted
     */
    void samplingTaskStarted();
    /**
     * @brief samplingSTasktopped is the callback after the sampling task stops
     *
     * @see #MainView::samplingTaskStopRequest
     * @see #MainView::controllingTaskStopped
     */
    void samplingTaskStopped();

private slots:
    void _uiBtnStartReleased();
    void _uiBtnChooseSampleDirReleased();
    void _uiTxtSampleDirChanged(const QString &dir);

private:
    explicit MainView(QWidget *parent=0);
    MainView(const MainView &) = delete;
    MainView &operator=(const MainView &) = delete;
    static MainView *createInstance();
    void closeEvent(QCloseEvent *e) override;

    Settings *_settings;
    WORK_STATUS _work_status;

    inline void _setupUI();
    inline void _makeConnections();
    QPushButton *_ui_btn_start;
    QPushButton *_ui_btn_monitor;
    QPushButton *_ui_btn_settings;
    QLabel      *_ui_lbl_video;
    QPlainTextEdit *_ui_txt_panel;
    QPushButton *_ui_btn_edit_gesture_list;
    QPushButton *_ui_btn_choose_sample_folder;
    QComboBox   *_ui_box_gesture_list;
    QLineEdit   *_ui_txt_sample_dir;
};

#endif