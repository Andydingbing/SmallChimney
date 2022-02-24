#ifndef Q_MODEL_FREQ_RES_H
#define Q_MODEL_FREQ_RES_H

#include "q_base_model.h"
#include "test_data.hpp"

// TS_T      : a test_report class
// TS_Data_T : a test_common_data class
template <typename TS_T, typename TS_Data_T>
class QwtFreqResData : public QwtTestData
{
public:
    explicit QwtFreqResData() :
        QwtTestData() {}

    TS_T *report() const
    { return _report; }

    void setReport(TS_T *report)
    { _report = report; }

    size_t size() const
    { return _report->data()->size(); }

    QPointF sample(size_t i) const
    {
        typename std::map<int64_t,TS_Data_T>::iterator iter;
        iter = _report->data()->begin();

        for (size_t j = 0;j < i;j ++) {
            iter ++;
        }

        return QPointF(iter->first/1e6,iter->second.pwr);
    }

private:
    TS_T *_report;
};

typedef QwtFreqResData<rd::rf_tx_freq_res_test,rd::rf_tx_freq_res_test_data> QwtRF_TX_FreqResData;
typedef QwtFreqResData<rd::if_tx_freq_res_test,rd::if_tx_freq_res_test_data> QwtIF_TX_FreqResData;
typedef QwtFreqResData<rd::rf_rx_freq_res_test,rd::rf_rx_freq_res_test_data> QwtRF_RX_FreqResData;
typedef QwtFreqResData<rd::if_rx_freq_res_test,rd::if_rx_freq_res_test_data> QwtIF_RX_FreqResData;

#endif // Q_MODEL_FREQ_RES_H
