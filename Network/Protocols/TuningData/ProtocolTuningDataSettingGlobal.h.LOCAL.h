#ifndef PROTOCOLTUNINGDATASETTINGGLOBAL_H
#define PROTOCOLTUNINGDATASETTINGGLOBAL_H

const int MAXIMUM_CONTENTS_COUNT = 1;

//000 ~099
enum TUNING_DATA_GLOBAL
{
    TUNING_DATA_GLOBAL_LAST_SELECTED_CONTENTS =0x01,
    TUNING_DATA_GLOBAL_EXPOSURE,
    TUNING_DATA_GLOBAL_GAIN,
    TUNING_DATA_GLOBAL_BRIGHTNESS,
    TUNING_DATA_GLOBAL_CONTRAST,
    TUNING_DATA_GLOBAL_SATURATION,
    TUNING_DATA_GLOBAL_SHARPNESS,
    TUNING_DATA_GLOBAL_WHITE_BALANCE,
    TUNING_DATA_GLOBAL_HUE,
    TUNING_DATA_GLOBAL_GAMMA,
    TUNING_DATA_GLOBAL_FOCUS,
    TUNING_DATA_GLOBAL_GUIDE_LINE_ROI_X1,
    TUNING_DATA_GLOBAL_GUIDE_LINE_ROI_X2,
    TUNING_DATA_GLOBAL_GUIDE_LINE_ROI_Y1,
    TUNING_DATA_GLOBAL_GUIDE_LINE_ROI_Y2,
    TUNING_DATA_GLOBAL_FLIP
};

//100,200,300,400
enum TUNING_DATA_CONTENTS_ANALYSIS
{
    TUNING_DATA_CONTENTS_ANALYSIS_HOUSING_ROI_X1,
    TUNING_DATA_CONTENTS_ANALYSIS_HOUSING_ROI_X2,
    TUNING_DATA_CONTENTS_ANALYSIS_HOUSING_ROI_Y1,
    TUNING_DATA_CONTENTS_ANALYSIS_HOUSING_ROI_Y2,
    TUNING_DATA_CONTENTS_ANALYSIS_INTENSITY_PATICLE,
    TUNING_DATA_CONTENTS_ANALYSIS_INTENSITY_BAND,
    TUNING_DATA_CONTENTS_ANALYSIS_INTENSITY_BACKGROUND,
    TUNING_DATA_CONTENTS_ANALYSIS_INTENSITY_WHITE,
    TUNING_DATA_CONTENTS_ANALYSIS_PATICLE_REMOVAL_REMOVAL_PATICLE,
    TUNING_DATA_CONTENTS_ANALYSIS_PATICLE_REMOVAL_AVERAGE_PATICLE,
    TUNING_DATA_CONTENTS_ANALYSIS_PATICLE_REMOVAL_THRESHOLD,
    TUNING_DATA_CONTENTS_ANALYSIS_BOX_CAR_WIDTH,
    TUNING_DATA_CONTENTS_ANALYSIS_BOX_CAR_HEIGHT,
    TUNING_DATA_CONTENTS_ANALYSIS_BOX_CAR_X_START,
    TUNING_DATA_CONTENTS_ANALYSIS_BOX_CAR_X_RANGE,
    TUNING_DATA_CONTENTS_ANALYSIS_BOX_CAR_PC_THRES,
    TUNING_DATA_CONTENTS_ANALYSIS_BOX_CAR_HOUSING_THRES,
    TUNING_DATA_CONTENTS_ANALYSIS_BAND_GAP_COUNT,
    TUNING_DATA_CONTENTS_ANALYSIS_USE_DUAL_BAND,
    TUNING_DATA_CONTENTS_ANALYSIS_DUAL_BAND_FRONT_BLOCK_COUNT,
    TUNING_DATA_CONTENTS_ANALYSIS_DUAL_BAND_X_START,
    TUNING_DATA_CONTENTS_ANALYSIS_DUAL_BAND_PC_THRES,
    TUNING_DATA_CONTENTS_ANALYSIS_PC_X_END,
    TUNING_DATA_CONTENTS_ANALYSIS_DUAL_BAND_PC_X_END,
    TUNING_DATA_CONTENTS_BOX_CAR_BAND_GAP_IDX
};


enum TUNING_CONTENTS_IDX
{
    TUNING_CONTENTS_IDX_GLOBAL = 1, // 1
    TUNING_CONTENTS_IDX_ANALYSIS_0, // 2
    TUNING_CONTENTS_IDX_END = 9
};


#endif // PROTOCOLTUNINGDATASETTINGGLOBAL_H
