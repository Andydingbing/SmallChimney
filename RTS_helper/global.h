#ifndef RTS_HELPER_GLOBAL_H
#define RTS_HELPER_GLOBAL_H

#ifdef RTS_HELPER_DLL_EXPORT
    #define RTS_HELPER_EXPORT Q_DECL_EXPORT
#else
    #define RTS_HELPER_EXPORT Q_DECL_IMPORT
#endif

#include "liblog.h"
#include "../instrument/instr.h"
#include "../instrument/libinstrument.h"
#include "device.h"

#include "freq_string.hpp"

#include "qwt_plot_helper.h"
#include "qwt_plot_curve.h"
#include "qwt_series_data.h"

#include "complex_sequence.h"
#include <QCheckBox>

#define CLR_PROTECT_EYE QColor(199,237,204)

#define COMBOBOX_MAP_FROM_BETTER_ENUM(box,better_enum) \
for (size_t i = 0;i < better_enum::_size();++i) { \
    box->addItem(QString::fromUtf8(better_enum::_names()[i])); \
}


template <typename T>
inline QString freqStringFrom(const T &freq, freq_string_unit_priority_t priority = FSU_M)
{ return QString::fromStdString(freq_string_from<T>(freq,priority)); }

inline QString DecimalToHexString(const qint8 dec)
{ return QString("0x%1").arg(dec,2,16,QLatin1Char('0')); }

inline QString DecimalToHexString(const quint8 dec)
{ return QString("0x%1").arg(dec,2,16,QLatin1Char('0')); }

inline QString DecimalToHexString(const qint16 dec)
{ return QString("0x%1").arg(dec,4,16,QLatin1Char('0')); }

inline QString DecimalToHexString(const quint16 dec)
{ return QString("0x%1").arg(dec,4,16,QLatin1Char('0')); }

inline QString DecimalToHexString(const qint32 dec)
{ return QString("0x%1").arg(dec,8,16,QLatin1Char('0')); }

inline QString DecimalToHexString(const quint32 dec)
{ return QString("0x%1").arg(dec,8,16,QLatin1Char('0')); }


class QTreeWidget;
class QTreeWidgetItem;

QTreeWidgetItem* selectFirst(QTreeWidget *tree);

#if defined (__cplusplus) || defined(__cplusplus__)
extern "C" {
#endif

RTS_HELPER_EXPORT quint32 currentRFIdx();
RTS_HELPER_EXPORT void setCurrentRFIdx(const quint32 idx);

extern complex_sequence *complexSequence;

#ifdef __cplusplus
}
#endif


#endif
