#ifndef DEFINE_H
#define DEFINE_H

#include "liblog.h"
#include "../instrument/instr.h"
#include "../instrument/libinstrument.h"
#include "device.h"

#include "freq_string.hpp"

#include "qwt_plot_helper.h"
#include "qwt_plot_curve.h"
#include "qwt_series_data.h"

#include "config_table.h"
#include "complex_sequence.h"
#include <QCheckBox>

enum Project {
    Ericsson_Radio_4415_B3,
    Ericsson_Radio_6449_B42,
    Ericsson_Air_3268_B47,
    StarPoint_SP9500,
    PROJECTS
};

enum Mode {
    Sequence,
    Manual
};

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
int checkedItems(const QTreeWidget *tree);
QTreeWidgetItem* selectFirst(QTreeWidget *tree);

// Common
extern Project project;
extern Mode mode;

extern quint32 RFIdx;
extern quint32 RFUIdx;
extern complex_sequence *complexSequence;

#endif
