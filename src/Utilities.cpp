#include "../include/Utilities.h"
namespace ocsort {
    Eigen::VectorXd ocsort::convert_bbox_to_z(Eigen::VectorXd bbox) {
        double w = bbox[2] - bbox[0];
        double h = bbox[3] - bbox[1];
        double x = bbox[0] + w / 2.0;
        double y = bbox[1] + h / 2.0;
        double s = w * h;
        double r = w / (h + 1e-6);
        Eigen::MatrixXd z(4, 1);
        z << x, y, s, r;
        return z;
    }
    Eigen::VectorXd ocsort::speed_direction(Eigen::VectorXd bbox1, Eigen::VectorXd bbox2) {
        double cx1 = (bbox1[0] + bbox1[2]) / 2.0;
        double cy1 = (bbox1[1] + bbox1[3]) / 2.0;
        double cx2 = (bbox2[0] + bbox2[2]) / 2.0;
        double cy2 = (bbox2[1] + bbox2[3]) / 2.0;
        Eigen::VectorXd speed(2, 1);
        speed << cy2 - cy1, cx2 - cx1;
        double norm = sqrt(pow(cy2 - cy1, 2) + pow(cx2 - cx1, 2)) + 1e-6;
        return speed / norm;
    }
    Eigen::VectorXd ocsort::convert_x_to_bbox(Eigen::VectorXd x) {
        float w = std::sqrt(x(2) * x(3));
        float h = x(2) / w;
        Eigen::VectorXd bbox = Eigen::VectorXd::Ones(4, 1);
        bbox << x(0) - w / 2, x(1) - h / 2, x(0) + w / 2, x(1) + h / 2;
        return bbox;
    }
    Eigen::VectorXd ocsort::k_previous_obs(std::unordered_map<int, Eigen::VectorXd> observations_, int cur_age, int k) {
        // 返回observations_中的某个观测值 (5,1)=>检测的坐标
        if (observations_.size() == 0) return Eigen::VectorXd::Constant(5, -1.0);
        for (int i = 0; i < k; i++) {
            int dt = k - i;// 这是 \Delta_t 求导数用
            if (observations_.count(cur_age - dt) > 0) return observations_.at(cur_age - dt);
        }
        auto iter = std::max_element(observations_.begin(), observations_.end(), [](const std::pair<int, Eigen::VectorXd> &p1, const std::pair<int, Eigen::VectorXd> &p2) { return p1.first < p2.first; });
        int max_age = iter->first;// 找出 map中最大的键key
        return observations_[max_age];
    }
}// namespace ocsort