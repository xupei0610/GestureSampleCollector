#ifndef HANDDETECTOR_H
#define HANDDETECTOR_H
/**
 * @file
 * @author Pei Xu, xupei0610 at gmail.com
 * @brief The HandDetector.h file contains the class of detecting hand region and generate a binary image of the hand.
 */
#include <QObject>
#include <QDebug>
#include <opencv2/opencv.hpp>
#include <vector>

#include "config.h"

/**
 * @brief The HandDetector class detects hand region andgenerate a binary image of the hand.
 *
 * **ATTENTION**:
 *  This function is not thread-safe.
 *
 * This class extracts gesture information by the function #HandDetector::detect
 *
 * The detection process includes
 *
 *  - image preprocessing
 *  - finger estimation
 *  - hand/gesture image extraction
 *
 * Basically, this function only check the area of the remaining region
 * after color filtering and some other basic image processing steps
 * and try to extract the hand region from the background
 * by estimating the location of wrist. It does not use a robust
 * algorithm to detect the hand in arbitrary background without calibration.
 *
 *
 * @see #HandDetector::detect
 */
class HandDetector : public QObject
{
    Q_OBJECT
public:
    /**
     * @brief COLOR_BLACK is the black color in BGR color space
     */
    const static cv::Scalar COLOR_BLACK;
    /**
     * @brief COLOR_WHITE is the white color in BGR color space
     */
    const static cv::Scalar COLOR_WHITE;
    /**
     * @brief COLOR_GRAY is the gray color in BGR color space
     */
    const static cv::Scalar COLOR_GRAY;
    /**
     * @brief COLOR_RED is the red color in BGR color space
     */
    const static cv::Scalar COLOR_RED;
    /**
     * @brief COLOR_GREEN is the green color in BGR color space
     */
    const static cv::Scalar COLOR_GREEN;
    /**
     * @brief COLOR_BLUE is the blue color in BGR color space
     */
    const static cv::Scalar COLOR_BLUE;
    /**
     * @brief interesting_img is a reference to #HandDetector::_interesting_img who is a copy of the current input image
     *
     * @see #HandDetector::_interesting_img
     */
    const cv::Mat &interesting_img;
    /**
     * @brief filtered_img is the image after preprocessing. This is a reference to #HandDetector::_filtered_img .
     *
     * The preprocessing includes
     *
     *  - background subtraction (if the background is set through #HandDetector::setBackgroundImage),
     *  - skin color filtering,
     *  - Gaussian blur,
     *  - thresholding, and
     *  - morphological transformation (if it is set through #HandDetector::setMorphology)
     *
     * The image obtained is a black-white image.
     *
     * @see #HandDetector::_filtered_img
     * @see #HandDetector::setBackgroundImage
     * @see #HandDetector::clearBackgroundImage
     * @see #HandDetector::setDetectionArea
     * @see #HandDetector::setMorphology
     * @see #HandDetector::detect
     */
    const cv::Mat &filtered_img;
    /**
     * @brief convexity_img is the image showing extracted contour region and convexity defects. This is a reference to #HandDetector::_convexity_img .
     *
     * A hand region and estimated hand center and finger tops would be drawn on the image.
     *
     * @see #HandDetector::_convexity_img
     * @see #HandDetector::detect
     */
    const cv::Mat &convexity_img;
    /**
     * @brief extracted_img is the image of the extracted hand region. This is a reference to #HandDetector::_extracted_img .
     *
     * This image is a black-white image, and only keeps the rectangle area of the hand region.
     * It may have different size.
     *
     * @see #HandDetector::_extracted_img
     * @see #HandDetector::detect
     */
    const cv::Mat &extracted_img;
    /**
     * @brief background_img is the background image used by the background subtractor.
     *
     * @see #HandDetector::_bg_subtractor
     * @see #HandDetector::filtered_img
     */
    const cv::Mat &background_img;

    /**
     * @brief skin_color_lower_bound is the lower bound for the skin color filter in HSV color space.
     *
     * It is a `cv::Scalar` with three elements representing h, s and v respectively in HSV color space.
     *
     * @see #HandDetector::filtered_img
     * @see #HandDetector::setSkinColorFilterLowerBound
     * @see #HandDetector::skin_color_upper_bound
     */
    const cv::Scalar &skin_color_lower_bound;
    /**
     * @brief skin_color_lower_bound is the upper bound for the skin color filter in HSV color space.
     *
     * It is a `cv::Scalar` with three elements representing h, s and v respectively in HSV color space.
     *
     * @see #HandDetector::filtered_img
     * @see #HandDetector::setSkinColorFilterUpperBound
     * @see #HandDetector::skin_color_lower_bound
     */
    const cv::Scalar &skin_color_upper_bound;

    /**
     * @brief morphology is the flag of performing morphology or not during image preprocessing.
     *
     * @see #HandDetector::setMorphology
     * @see #HandDetector::filtered_img
     */
    const bool &morphology;
    /**
     * @brief detection_area is the minimum area of a contour region who will be considered as a hand region.
     *
     * @see #HandDetector::setDetectionArea
     */
    const int &detection_area;
    /**
     * @brief waitting_bg is a indicator if the system is waitting for setting the background image for the brackground subtractor.
     *
     * @see #HandDetector::_bg_subtractor
     */
    const bool &waitting_bg;

    explicit HandDetector(QObject *parent = 0);
    /**
     * @brief detect detects hand and fingers from the given image
     *
     * The detection process includes
     *
     *  - image preprocessing
     *  - finger estimation
     *  - hand/gesture image extraction
     *
     * Basically, this function only check the area of the remaining region
     * after color filtering and some other basic image processing steps
     * and try to extract the hand region from the background
     * by estimating the location of wrist. It does not use a robust
     * algorithm to detect the hand in arbitrary background without calibration.
     *
     * @param input_img : an image
     * @retval true : if detect something
     * @retval false : if nothing detected
     *
     * @see #HandDetector::filtered_img
     * @see #HandDetector::convexity_img
     * @see #HandDetector::extracted_img
     */
    bool detect(const cv::Mat &input_img);

signals:
    /**
     * @brief backgroundImageSet is the signal to indicate a new background image for the background subtractor being set.
     *        Use #HandDetector::background_img to retrieve the new background image.
     *
     * @see #HandDetector::setBackgroundImage
     * @see #HandDetector::backgroundImageCleared
     * @see #HandDetector::waiting_bg
     */
    void backgroundImageSet();
    /**
     * @brief backgroundImageCleared is the signal to indicate the background image set for the background subtractor has been cleared.
     *
     * @see #HandDetector::clearBackgroundImage
     * @see #HandDetector::backgroundImageSet
     */
    void backgroundImageCleared();

public slots:
    /**
     * @brief setMorphology sets the flag of performing the morphological transformation.
     * @param perform_morphology : the flag of performing the morphological transformation or not.
     *
     * @see #HandDetector::filtered_img
     * @see #HandDetector::morphology
     */
    void setMorphology(const bool & perform_morphology);
    /**
     * @brief setSkinColorFilterLowerBound sets the lower bound for the skin color filter in HSV color space.
     * @param H : hue in HSV color space, in range 0 to 255
     * @param S : saturation in HSV color space, in range 0 to 255
     * @param V : value in HSV color space, in range 0 to 255
     *
     * @see #HandDetector::filtered_img
     * @see #HandDetector::skin_color_lower_bound
     * @see #HandDetector::setSkinColorFilterUpperBound
     */
    void setSkinColorFilterLowerBound(const int & H, const int & S, const int & V);
    /**
     * @brief setSkinColorFilterUpperBound sets the uperer bound for the skin color filter in HSV color space.
     * @param H : hue in HSV color space, in range 0 to 255
     * @param S : saturation in HSV color space, in range 0 to 255
     * @param V : value in HSV color space, in range 0 to 255
     *
     * @see #HandDetector::filtered_img
     * @see #HandDetector::skin_color_upper_bound
     * @see #HandDetector::setSkinColorFilterLowerBound
     */
    void setSkinColorFilterUpperBound(const int & H, const int & S, const int & V);
    /**
     * @brief setDetectionArea sets the minimum area of a contour region who will be considered as a hand region.
     * @param area : the minimum area
     *
     * @see #HandDetector::filtered_img
     * @see #HandDetector::detection_area
     */
    void setDetectionArea(const int & area);
    /**
     * @brief setBackgroundImage indicates the class to set the last input image as the background image for the background subtractor.
     *
     * @see #HandDetector::filtered_img
     * @see #HandDetector::backgroundImageSet
     * @see #HandDetector::_bg_subtractor
     * @see #HandDetector::clearBackgroundImage
     */
    void setBackgroundImage();
    /**
     * @brief clearBackgroundImage clears the background image for the background subtractor.
     *        No background subtractor will be used after this function is called.
     *
     * @see #HandDetector::filtered_img
     * @see #HandDetector::backgroundImageCleared
     * @see #HandDetector::_bg_subtractor
     * @see #HandDetector::setBackgroundImage
     */
    void clearBackgroundImage();

protected:
    /**
     * @brief _interesting_img is a copy of the current input image
     *
     * @see #HandDetector::interesting_img
     */
    cv::Mat _interesting_img;
    /**
     * @brief _filtered_img is is the image after preprocessing.
     *
     * @see #HandDetector::filtered_img
     */
    cv::Mat _filtered_img;
    /**
     * @brief _convexity_img is the image showing extracted contour region and convexity defects.
     *
     * @see #HandDetector::convexity_img
     */
    cv::Mat _convexity_img;
    /**
     * @brief _extracted_img is the image of the extracted hand region.
     *
     * @see #HandDetector::extracted_img
     */
    cv::Mat _extracted_img;
    /**
     * @brief _bg_subtractor is the background subtractor used.
     *
     * @see #HandDetector::waitting_bg
     * @see #HandDetector::setBackgroundImage
     * @see #HandDetector::clearBackgroundImage
     */
    cv::Ptr<cv::BackgroundSubtractor> _bg_subtractor;

private:
    cv::Mat _background_img; // a copy of the initial background image
    cv::Mat _bg; // used as the mask for subtractor
    bool _has_set_bg;
    bool _waitting_bg;

    cv::Scalar _skin_color_lower_bound;
    cv::Scalar _skin_color_upper_bound;

    cv::Size _gaussian_size;
    double _gaussian_variance;

    bool _morphology;
    cv::Mat _morphology_kernel;

    int  _detection_area;

    inline void _processImage();
    inline bool _extractHand();
    template <typename T1, typename T2>
    inline double _squaredEuclidDist(const T1 &p1, const T2 &p2) const;
};

#endif // HANDDETECTOR_H
