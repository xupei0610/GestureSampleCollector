#include "SampleCollector.hpp"

// const cv::Size SampleCollector::sample_image_size(SAMPLE_SIZE_WIDTH,SAMPLE_SIZE_HEIGHT);
const char *SampleCollector::orig_image_format = SAMPLE_ORIG_FORMAT;
const char *SampleCollector::proc_image_format = SAMPLE_PROC_FORMAT;

SampleCollector::SampleCollector(QObject *parent) :
    QObject(parent),
    storage_path(_storage_path),
    _sampling_timer(new QTimer),
    _settings(Settings::getInstance()),
    _storage_dir(nullptr),
    _storage_dir_orig(nullptr),
    _storage_dir_proc(nullptr)
{
    _sampling_timer->setSingleShot(true);
}

SampleCollector::~SampleCollector()
{
    if (_storage_dir != nullptr)
        delete _storage_dir;
    if (_storage_dir_orig != nullptr)
        delete _storage_dir_orig;
    if (_storage_dir_proc != nullptr)
        delete _storage_dir_proc;
    delete _sampling_timer;
}

// cv::Mat SampleCollector::resizeSample(const cv::Mat &sample)
// {
//     if (sample.empty() || (sample.rows == sample_image_size.height && sample.cols == sample_image_size.width))
//         return cv::Mat(sample);

//     cv::Mat result(sample_image_size, CV_8UC1);
//     float scale_x = (float)sample_image_size.width/sample.cols;
//     float scale_y = (float)sample_image_size.height/sample.rows;
//     if (scale_x == scale_y)
//     {
//         cv::resize(sample, result, sample_image_size, 0, 0, cv::INTER_LINEAR);
//         return result;
//     }

//     result.setTo(cv::Scalar(0));
//     int x = 0, y = 0;
//     cv::Size size;
//     if (scale_x < scale_y)
//     {
//         size.width = sample_image_size.width;
//         size.height = std::ceil(sample.rows*scale_x);
//         y = (sample_image_size.height-size.height)/2;
//     }
//     else
//     {
//         size.height = sample_image_size.height;
//         size.width  = std::ceil(sample.cols*scale_y);
//         x = (sample_image_size.width-size.width)/2;
//     }
//     cv::Mat tmp;
//     cv::resize(sample, tmp, size, 0, 0, cv::INTER_LINEAR);
//     tmp.copyTo(result(cv::Rect(x,y,size.width,size.height)));

//     return result;
// }

bool SampleCollector::setStoragePath(const QString &sample_folder, const QString &label_name)
{
    if (sample_folder.isEmpty())
        return false;

    auto dir = QDir(sample_folder);
    if (dir.isRoot())
        return false;

    if (_storage_dir == nullptr)
        _storage_dir = new QDir(dir.filePath(label_name));
    else
        _storage_dir->setPath(dir.filePath(label_name));
    if (_storage_dir_orig == nullptr)
        _storage_dir_orig = new QDir(_storage_dir->filePath(SAMPLE_ORIG_FORMAT));
    else
        _storage_dir_orig->setPath(_storage_dir->filePath(SAMPLE_ORIG_FORMAT));
    if (_storage_dir_proc == nullptr)
        _storage_dir_proc = new QDir(_storage_dir->filePath(SAMPLE_PROC_FORMAT));
    else
        _storage_dir_proc->setPath(_storage_dir->filePath(SAMPLE_PROC_FORMAT));

    _storage_path = _storage_dir->absolutePath();

    if (dir.exists() &&
        (_storage_dir->exists() || dir.mkdir(label_name)) &&
        (_storage_dir_orig->exists() || _storage_dir->mkdir(SAMPLE_ORIG_FORMAT)) &&
        (_storage_dir_proc->exists() || _storage_dir->mkdir(SAMPLE_PROC_FORMAT))
       )
        return true;

    return false;
}

bool SampleCollector::sample(const cv::Mat &orig_img, const cv::Mat &proc_img)
{
    if (orig_img.empty() || proc_img.empty())
        return false;

    QPixmap orig_image = CvQtImgConvertor::cvMat2QPixmap(orig_img);
    // QPixmap proc_image = CvQtImgConvertor::cvMat2QPixmap(resizeSample(proc_img));
    QPixmap proc_image = CvQtImgConvertor::cvMat2QPixmap(proc_img);

    QString file_name = QString::number(qrand());
    while (true)
    {
        if (_storage_dir_orig->exists(file_name) || _storage_dir_proc->exists(file_name))
            file_name = QString::number(qrand());
        else
        {
            if (orig_image.save(_storage_dir_orig->filePath(file_name), SAMPLE_ORIG_FORMAT) &&
                proc_image.save(_storage_dir_proc->filePath(file_name), SAMPLE_PROC_FORMAT)
               )
            {
                _sampling_timer->start(_settings->sampling_interval);
                return true;
            }
            return false;
        }
    }
    return true;
}

bool SampleCollector::deny()
{
    return _sampling_timer->isActive();
}
