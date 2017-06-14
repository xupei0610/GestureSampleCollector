#ifndef GESTURE_SAMPLE_COLLECTOR_CONFIG_H
#define GESTURE_SAMPLE_COLLECTOR_CONFIG_H

/**
 * @file
 * @author Pei Xu, xupei0610 at gmail.com
 * @brief The config.h file defines the default paramters to initialize the system.
 */

// @cond
#define SETTING_STRING_DELIMITER "!x0x?"
// @endcond
// @cond
#define STR_HELPER(X) #X
// @endcond
/**
 * @brief STR is a helper to convert a macro defined variable to a string.
 */
#define STR(X) STR_HELPER(X)

/**
 * @brief VERSION_MAJOR is the major version number of this application.
 */
#define VERSION_MAJOR 1
/**
 * @brief VERSION_MINOR is the minor version number of this application.
 */
#define VERSION_MINOR 0
/**
 * @brief VERSION is the current version of this application.
 */
#define VERSION STR(VERSION_MAJOR) "." STR(VERSION_MINOR)

/**
 * @brief SETTING_SCOPE is the valid scope of the setting file.
 */
#define SETTING_SCOPE QSettings::UserScope
/**
 * @brief SETTING_ORGANIZATION_NAME is the name of developer.
 */
#define SETTING_ORGANIZATION_NAME "PeiXu"
/**
 * @brief SETTING_APPLICATION_NAME is the name of the system.
 */
#define SETTING_APPLICATION_NAME "GestureSampleCollector"
#ifndef CAMERA_FPS
/**
 * @brief CAMERA_FPS is the FPS of the camera
 */
#  define CAMERA_FPS 50
#endif
#ifndef DEFAULT_ROI_MARGIN_LEFT
/**
 * @brief DEFAULT_ROI_MARGIN_LEFT is the default left margin, in pixel, of the region of interesting on the frame captured by the camera.
 */
#  define DEFAULT_ROI_MARGIN_LEFT 50
#endif
#ifndef DEFAULT_ROI_MARGIN_RIGHT
/**
 * @brief DEFAULT_ROI_MARGIN_RIGHT is the default right margin, in pixel, of the region of interesting on the frame captured by the camera.
 */
#  define DEFAULT_ROI_MARGIN_RIGHT 50
#endif
#ifndef DEFAULT_ROI_MARGIN_TOP
/**
 * @brief DEFAULT_ROI_MARGIN_TOP is the default top margin, in pixel, of the region of interesting on the frame captured by the camera.
 */
#  define DEFAULT_ROI_MARGIN_TOP 10
#endif
#ifndef DEFAULT_ROI_MARGIN_BOTTOM
/**
 * @brief DEFAULT_ROI_MARGIN_BOTTOM is the default bottom margin, in pixel, of the region of interesting on the frame captured by the camera.
 */
#  define DEFAULT_ROI_MARGIN_BOTTOM 150
#endif
#ifndef DEFAULT_ROI_START_X
/**
 * @brief DEFAULT_ROI_START_X is the default left bound, in percent, of the region of interesting on the frame captured by the camera.
 */
#  define DEFAULT_ROI_START_X 48
#endif
#ifndef DEFAULT_ROI_END_Y
/**
 * @brief DEFAULT_ROI_END_X is the default right bound, in percent, of the region of interesting on the frame captured by the camera.
 */
#  define DEFAULT_ROI_END_X 98
#endif
#ifndef DEFAULT_ROI_START_Y
/**
 * @brief DEFAULT_ROI_START_Y is the default top bound, in percent, of the region of interesting on the frame captured by the camera.
 */
#  define DEFAULT_ROI_START_Y 2
#endif
#ifndef DEFAULT_ROI_END_Y
/**
 * @brief DEFAULT_ROI_END_Y is the default bottom bound, in percent, of the region of interesting on the frame captured by the camera.
 */
#  define DEFAULT_ROI_END_Y 68
#endif

#ifndef DEFAULT_SKIN_COLOR_MIN_H
/**
  * @brief DEFAULT_SKIN_COLOR_MIN_H is the default lower bound of the skin color filter for hue in HSV color space.
  */
#  define DEFAULT_SKIN_COLOR_MIN_H 30
#endif
#ifndef DEFAULT_SKIN_COLOR_MAX_H
/**
  * @brief DEFAULT_SKIN_COLOR_MAX_H is the default upper bound of the skin color filter for hue in HSV color space.
  */
#  define DEFAULT_SKIN_COLOR_MAX_H 180
#endif
#ifndef DEFAULT_SKIN_COLOR_MIN_S
/**
  * @brief DEFAULT_SKIN_COLOR_MIN_S is the default lower bound of the skin color filter for saturation in HSV color space.
  */
#  define DEFAULT_SKIN_COLOR_MIN_S 0
#endif
#ifndef DEFAULT_SKIN_COLOR_MAX_S
/**
  * @brief DEFAULT_SKIN_COLOR_MAX_S is the default upper bound of the skin color filter for saturation in HSV color space.
  */
#  define DEFAULT_SKIN_COLOR_MAX_S 255
#endif
#ifndef DEFAULT_SKIN_COLOR_MIN_V
/**
  * @brief DEFAULT_SKIN_COLOR_MIN_V is the default lower bound of the skin color filter for value in HSV color space.
  */
#  define DEFAULT_SKIN_COLOR_MIN_V 100
#endif
#ifndef DEFAULT_SKIN_COLOR_MAX_V
/**
  * @brief DEFAULT_SKIN_COLOR_MAX_V is the default upper bound of the skin color filter for value in HSV color space.
  */
#  define DEFAULT_SKIN_COLOR_MAX_V 255
#endif

#ifndef DEFAULT_SKIN_DETECTION_AREA
/**
 * @brief DEFAULT_SKIN_DETECTION_AREA is the default minimum area of a hand contour who is considered as a hand.
 */
#  define DEFAULT_SKIN_DETECTION_AREA 5000
#endif
#ifndef DEFAULT_SKIN_MORPHOLOGY
/**
 * @brief DEFAULT_SKIN_MORPHOLOGY is the default flag, boolean value, if the morphological transformation is performed or not.
 */
#  define DEFAULT_SKIN_MORPHOLOGY true
#endif

#ifndef DEFAULT_SAMPLING_AMOUNT_PER_TIME
/**
 * @brief DEFAULT_SAMPLING_AMOUNT_PER_TIME is the amount of samples collected once.
 */
#  define DEFAULT_SAMPLING_AMOUNT_PER_TIME 100
#endif
#ifndef DEFAULT_SAMPLING_INTERVAL
/**
 * @brief DEFAULT_SAMPLING_INTERVAL is the minimum interval, in ms, between two sampling actions.
 */
#  define DEFAULT_SAMPLING_INTERVAL 150
#endif

// #ifndef SAMPLE_SIZE_WIDTH
// /**
//  * @brief SAMPLE_SIZE_WIDTH is the width of the sample image.
//  *
//  * Change in this parameter may cause invalidity of samples collected previously.
//  */
// #  define SAMPLE_SIZE_WIDTH 128
// #endif
// #ifndef SAMPLE_SIZE_HEIGHT
// /**
//  * @brief SAMPLE_SIZE_HEIGHT is the height of the sample image.
//  *
//  * Change in this parameter may cause invalidity of samples collected previously.
//  */
// #  define SAMPLE_SIZE_HEIGHT 128
// #endif
#ifndef SAMPLE_ORIG_FORMAT
/**
 * @brief SAMPLE_ORIG_FORMAT is the format of the sample image.
 *
 * Change in this parameter may cause invalidity of samples collected previously.
 */
#  define SAMPLE_ORIG_FORMAT "BMP"
#endif
#ifndef SAMPLE_PROC_FORMAT
/**
 * @brief SAMPLE_PROC_FORMAT is the format of the sample image.
 *
 * Change in this parameter may cause invalidity of samples collected previously.
 */
#  define SAMPLE_PROC_FORMAT "PGM"
#endif
/**
 * @brief INITIAL_GESTURE_LISTS is a default list of 47 gestures
 */
#define INITIAL_GESTURE_LISTS    "0" SETTING_STRING_DELIMITER \
  "1" SETTING_STRING_DELIMITER "2" SETTING_STRING_DELIMITER \
  "3" SETTING_STRING_DELIMITER "4" SETTING_STRING_DELIMITER \
  "5" SETTING_STRING_DELIMITER "6" SETTING_STRING_DELIMITER \
  "7" SETTING_STRING_DELIMITER "8" SETTING_STRING_DELIMITER \
  "9" SETTING_STRING_DELIMITER "A" SETTING_STRING_DELIMITER \
  "B" SETTING_STRING_DELIMITER "C" SETTING_STRING_DELIMITER \
  "D" SETTING_STRING_DELIMITER "E" SETTING_STRING_DELIMITER \
  "F" SETTING_STRING_DELIMITER "G" SETTING_STRING_DELIMITER \
  "H" SETTING_STRING_DELIMITER "I" SETTING_STRING_DELIMITER \
  "J" SETTING_STRING_DELIMITER "K" SETTING_STRING_DELIMITER \
  "L" SETTING_STRING_DELIMITER "M" SETTING_STRING_DELIMITER \
  "N" SETTING_STRING_DELIMITER "O" SETTING_STRING_DELIMITER \
  "P" SETTING_STRING_DELIMITER "Q" SETTING_STRING_DELIMITER \
  "R" SETTING_STRING_DELIMITER "S" SETTING_STRING_DELIMITER \
  "T" SETTING_STRING_DELIMITER "U" SETTING_STRING_DELIMITER \
  "V" SETTING_STRING_DELIMITER "W" SETTING_STRING_DELIMITER \
  "X" SETTING_STRING_DELIMITER "Y" SETTING_STRING_DELIMITER \
  "Z" SETTING_STRING_DELIMITER "Z0" SETTING_STRING_DELIMITER \
  "Z1" SETTING_STRING_DELIMITER "Z2" SETTING_STRING_DELIMITER \
  "Z3" SETTING_STRING_DELIMITER "Z4" SETTING_STRING_DELIMITER \
  "Z5" SETTING_STRING_DELIMITER



#endif
