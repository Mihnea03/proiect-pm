#include "face_utils.h"
#include "esp_camera.h"
#include "dl_lib_matrix3d.h"
#include "img_converters.h"
#include "esp_log.h"

static const char* TAG = "face_utils";

mtmn_config_t mtmn_config;

void init_face_detection() {
    mtmn_config.type = FAST;
    mtmn_config.min_face = 80;
    mtmn_config.pyramid = 0.707;
    mtmn_config.pyramid_times = 4;

    mtmn_config.p_threshold.score = 0.6;
    mtmn_config.p_threshold.nms = 0.7;
    mtmn_config.p_threshold.candidate_number = 20;

    mtmn_config.r_threshold.score = 0.7;
    mtmn_config.r_threshold.nms = 0.7;
    mtmn_config.r_threshold.candidate_number = 10;

    mtmn_config.o_threshold.score = 0.7;
    mtmn_config.o_threshold.nms = 0.7;
    mtmn_config.o_threshold.candidate_number = 1;
}

bool isFaceDetected() {
    camera_fb_t *fb = esp_camera_fb_get();
    if (!fb || fb->width > 400) {
        if (fb) esp_camera_fb_return(fb);
        return false;
    }

    dl_matrix3du_t *image_matrix = dl_matrix3du_alloc(1, fb->width, fb->height, 3);
    if (!image_matrix) {
        esp_camera_fb_return(fb);
        return false;
    }

    bool ok = fmt2rgb888(fb->buf, fb->len, fb->format, image_matrix->item);
    esp_camera_fb_return(fb);
    if (!ok) {
        dl_matrix3du_free(image_matrix);
        return false;
    }

    box_array_t *net_boxes = face_detect(image_matrix, &mtmn_config);
    bool found = (net_boxes && net_boxes->len > 0);

    if (net_boxes) {
        dl_lib_free(net_boxes->score);
        dl_lib_free(net_boxes->box);
        if (net_boxes->landmark) dl_lib_free(net_boxes->landmark);
        dl_lib_free(net_boxes);
    }

    dl_matrix3du_free(image_matrix);
    return found;
}
