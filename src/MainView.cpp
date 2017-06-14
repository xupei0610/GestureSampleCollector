#include "MainView.hpp"

MainView *MainView::getInstance()
{
    return Singleton<MainView>::instance(MainView::createInstance);
}

MainView *MainView::createInstance()
{
    return new MainView;
}

MainView::MainView(QWidget *parent) :
    QDialog(parent),
    work_status(_work_status),
    _settings(Settings::getInstance()),
    _work_status(STATUS_WAITING_CAMERA)
{
    _setupUI();
    _makeConnections();
    _ui_box_gesture_list->addItems(_settings->gesture_list);
    if (_settings->gesture_list.size() > _settings->gesture_selected && _settings->gesture_selected > 0)
        _ui_box_gesture_list->setCurrentIndex(_settings->gesture_selected);
    _ui_txt_sample_dir->setText(_settings->sample_storage_path);
}

void MainView::updateVideoFrame(const QPixmap &img)
{
    _ui_lbl_video->setPixmap(
                img.scaled(_ui_lbl_video->width(),
                           _ui_lbl_video->height(),
                           Qt::KeepAspectRatio)
                );
}

int MainView::getVideoFrameWidth()
{
    return _ui_lbl_video->width();
}

int MainView::getVideoFrameHeight()
{
    return _ui_lbl_video->height();
}

void MainView::appendText(const QString &text)
{
    _ui_txt_panel->appendPlainText(text);
}

void MainView::updateText(const QString &new_text)
{
    QTextCursor cursor = _ui_txt_panel->textCursor();
    _ui_txt_panel->moveCursor(QTextCursor::End, QTextCursor::MoveAnchor);
    _ui_txt_panel->moveCursor(QTextCursor::StartOfLine, QTextCursor::MoveAnchor);
    _ui_txt_panel->moveCursor(QTextCursor::End, QTextCursor::KeepAnchor);
    _ui_txt_panel->textCursor().removeSelectedText();
    _ui_txt_panel->textCursor().deletePreviousChar();
    _ui_txt_panel->setTextCursor(cursor);
    _ui_txt_panel->appendPlainText(new_text);
}

void MainView::clearText()
{
    _ui_txt_panel->clear();
}

void MainView::setText(const QString &text)
{
    _ui_txt_panel->setPlainText(text);
}

void MainView::reloadLabelList()
{
    _ui_box_gesture_list->clear();
    _ui_box_gesture_list->addItems(_settings->gesture_list);
}

void MainView::cameraStarted()
{
    _ui_btn_start->setEnabled(true);
    _ui_btn_start->setText(tr("Sample"));
    _ui_box_gesture_list->setEnabled(true);
    _ui_btn_edit_gesture_list->setEnabled(true);
    _ui_txt_sample_dir->setEnabled(true);
    _ui_btn_choose_sample_folder->setEnabled(true);
    _work_status = STATUS_IDLE;
}

void MainView::cameraReleased()
{
    if (_work_status == STATUS_WORKING)
        samplingTaskStopped();
    _work_status = STATUS_WAITING_CAMERA;
    _ui_btn_start->setText(tr("Launch Camera"));
}

void MainView::samplingTaskStarted()
{
    _ui_btn_start->setText(tr("Stop"));
    _ui_btn_start->setEnabled(true);
    _work_status = STATUS_WORKING;
}

void MainView::samplingTaskStopped()
{
    _ui_box_gesture_list->setEnabled(true);
    _ui_btn_edit_gesture_list->setEnabled(true);
    _ui_txt_sample_dir->setEnabled(true);
    _ui_btn_choose_sample_folder->setEnabled(true);
    _ui_btn_start->setEnabled(true);
    if (_work_status != STATUS_WAITING_CAMERA)
    {
        _work_status = STATUS_IDLE;
        _ui_btn_start->setText(tr("Sample"));
    }
}

void MainView::_uiBtnStartReleased()
{
    _ui_btn_start->setEnabled(false);
    _ui_box_gesture_list->setEnabled(false);
    _ui_btn_edit_gesture_list->setEnabled(false);
    _ui_txt_sample_dir->setEnabled(false);
    _ui_btn_choose_sample_folder->setEnabled(false);
    if (_work_status == STATUS_WAITING_CAMERA)
        emit cameraRequest();
    else if (_work_status == STATUS_IDLE)
        emit samplingTaskStartRequest(_ui_box_gesture_list->currentIndex(), _ui_txt_sample_dir->text());
    else
        emit samplingTaskStopRequest();
}

void MainView::_uiBtnChooseSampleDirReleased()
{
    QString dir = _ui_txt_sample_dir->text();
    dir = QFileDialog::getExistingDirectory(this, tr("Select Sample Storage Path"),
                                            (!dir.isEmpty() && QFileInfo(dir).isDir()) ? dir : QDir::homePath(),
                                            QFileDialog::ShowDirsOnly|QFileDialog::DontUseNativeDialog);
    if (!dir.isEmpty())
        _ui_txt_sample_dir->setText(dir);
}

void MainView::_uiTxtSampleDirChanged(const QString & dir)
{
    if (!dir.isEmpty())
    {
        if (QFileInfo(dir).isDir())
            _ui_txt_sample_dir->setStyleSheet("QLineEdit {color: auto}");
        else
            _ui_txt_sample_dir->setStyleSheet("QLineEdit {color: red}");
    }
}

void MainView::closeEvent(QCloseEvent *e)
{
    emit mainViewClosing();
    // e->accept();
}

void MainView::_setupUI()
{
    _ui_lbl_video = new QLabel;
    _ui_lbl_video->setFixedSize(640, 480);
    _ui_lbl_video->setSizePolicy(QSizePolicy::Fixed, QSizePolicy::Fixed);

    _ui_txt_panel = new QPlainTextEdit;
    _ui_txt_panel->setFixedSize(250, 300);
    _ui_txt_panel->setSizePolicy(QSizePolicy::Fixed, QSizePolicy::Fixed);
    _ui_txt_panel->setReadOnly(true);

    _ui_btn_start = new QPushButton(tr("Launch Camera"));
    _ui_btn_monitor = new QPushButton(tr("Monitor"));
    _ui_btn_settings = new QPushButton(tr("Settings"));
    _ui_btn_start->setSizePolicy(QSizePolicy::Fixed, QSizePolicy::Fixed);
    _ui_btn_start->setAutoDefault(false);
    _ui_btn_start->setFocusPolicy(Qt::NoFocus);
    _ui_btn_monitor->setSizePolicy(QSizePolicy::Fixed, QSizePolicy::Fixed);
    _ui_btn_monitor->setAutoDefault(false);
    _ui_btn_monitor->setFocusPolicy(Qt::NoFocus);
    _ui_btn_monitor->setToolTip(tr("Show monitor window"));
    _ui_btn_settings->setSizePolicy(QSizePolicy::Fixed, QSizePolicy::Fixed);
    _ui_btn_settings->setAutoDefault(false);
    _ui_btn_settings->setFocusPolicy(Qt::NoFocus);
    _ui_btn_settings->setToolTip(tr("Show setting window"));

    QLabel *ui_lbl_gesture = new QLabel(tr("Gesture"));
    QLabel *ui_lbl_sample_folder = new QLabel(tr("Store to"));
    ui_lbl_gesture->setSizePolicy(QSizePolicy::Fixed, QSizePolicy::Fixed);
    ui_lbl_sample_folder->setSizePolicy(QSizePolicy::Fixed, QSizePolicy::Fixed);
    _ui_box_gesture_list = new QComboBox;
    _ui_txt_sample_dir = new QLineEdit;
    _ui_box_gesture_list->setSizePolicy(QSizePolicy::Expanding, QSizePolicy::Fixed);
    _ui_txt_sample_dir->setSizePolicy(QSizePolicy::Expanding, QSizePolicy::Fixed);
    _ui_btn_edit_gesture_list = new QPushButton("...");
    _ui_btn_choose_sample_folder = new QPushButton("...");
    _ui_btn_edit_gesture_list->setSizePolicy(QSizePolicy::Fixed, QSizePolicy::Fixed);
    _ui_btn_edit_gesture_list->setAutoDefault(false);
    _ui_btn_edit_gesture_list->setFocusPolicy(Qt::NoFocus);
    _ui_btn_edit_gesture_list->setToolTip(tr("Edit"));
    _ui_btn_choose_sample_folder->setSizePolicy(QSizePolicy::Fixed, QSizePolicy::Fixed);
    _ui_btn_choose_sample_folder->setAutoDefault(false);
    _ui_btn_choose_sample_folder->setFocusPolicy(Qt::NoFocus);
    _ui_btn_choose_sample_folder->setToolTip(tr("Browse"));
    QGridLayout *ui_sample_form = new QGridLayout;
    ui_sample_form->setContentsMargins(0, 0, 0, 0);
    ui_sample_form->setSpacing(5);
    ui_sample_form->addWidget(ui_lbl_gesture,               0, 0, 1, 1, Qt::AlignRight);
    ui_sample_form->addWidget(_ui_box_gesture_list,         0, 1, 1, 1);
    ui_sample_form->addWidget(_ui_btn_edit_gesture_list,    0, 2, 1, 1);
    ui_sample_form->addWidget(ui_lbl_sample_folder,         1, 0, 1, 1, Qt::AlignRight);
    ui_sample_form->addWidget(_ui_txt_sample_dir,           1, 1, 1, 1);
    ui_sample_form->addWidget(_ui_btn_choose_sample_folder, 1, 2, 1, 1);

    QGridLayout *main_layout = new QGridLayout;
    main_layout->setContentsMargins(20, 20, 20, 20);
    main_layout->setSpacing(20);
    main_layout->addWidget(_ui_lbl_video,   0, 0, 4, 1, Qt::AlignCenter);
    main_layout->addWidget(_ui_txt_panel,   0, 1, 1, 2);
    main_layout->addWidget(_ui_btn_start,   1, 1, 1, 1);
    main_layout->addLayout(ui_sample_form,  2, 1, 1, 2);
    main_layout->addWidget(_ui_btn_settings,3, 1, 1, 1);
    main_layout->addWidget(_ui_btn_monitor, 3, 2, 1, 1, Qt::AlignLeft);

    setLayout(main_layout);
    setFixedSize(minimumSize());

    setWindowTitle(tr("Gesture Simple Collector"));
}

void MainView::_makeConnections()
{
    connect(_ui_btn_monitor, SIGNAL(released()), this, SIGNAL(monitorWindowRequest()));
    connect(_ui_btn_settings, SIGNAL(released()), this, SIGNAL(settingsWindowRequest()));
    connect(_ui_btn_edit_gesture_list, SIGNAL(released()), this, SIGNAL(settingsWindowGestureTabRequest()));
    connect(_ui_btn_start, SIGNAL(released()), this, SLOT(_uiBtnStartReleased()));
    connect(_ui_btn_choose_sample_folder, SIGNAL(released()), this, SLOT(_uiBtnChooseSampleDirReleased()));                              
    connect(_ui_txt_sample_dir, SIGNAL(textChanged(QString)), this, SLOT(_uiTxtSampleDirChanged(QString)));
}
