#ifndef EUTRA_BAND_H
#define EUTRA_BAND_H

#include "global_header.h"
#include "../utilities/range.hpp"
#include "freq_string.hpp"

enum band_usage_t {
    Band_Normal,
    Band_Reserved,
    Band_NA
};

class band_range_t : public range<uint64_t,uint64_t>
{
public:
    typedef range<uint64_t,uint64_t> base_type;

    band_range_t() : base_type()
    { _usage = Band_Normal; }

    band_range_t(const uint64_t low,const uint64_t high) : base_type(low,high)
    { _usage = Band_Normal; }

    band_range_t(const band_usage_t usage,const uint64_t low = 0,const uint64_t high = 0)
    {
        _usage = usage;

        if (usage == Band_Normal) {
            _low = low;
            _high = high;
        } else {
            _low = _high = 0;
        }
    }

    std::string str() const
    {
        if (_usage == Band_Reserved) {
            return "Reserved";
        }

        if (_usage == Band_NA) {
            return "N/A";
        }

        return freq_string_from(_low) + " ~ " + freq_string_from(_high);
    }

public:
    band_usage_t _usage;
};

namespace ns_eutra {

class band
{
public:
    typedef band_range_t range;
    band(const uint8_t operating_band) : _band(operating_band) {}

    range ul_range() const
    {
        switch (_band) {
        case 1  : return range(FREQ_M(1920),FREQ_M(1980));
        case 2  : return range(FREQ_M(1850),FREQ_M(1910));
        case 3  : return range(FREQ_M(1710),FREQ_M(1785));
        case 4  : return range(FREQ_M(1710),FREQ_M(1755));
        case 5  : return range(FREQ_M(824),FREQ_M(849));
        case 6  : return range(FREQ_M(830),FREQ_M(840));
        case 7  : return range(FREQ_M(2500),FREQ_M(2570));
        case 8  : return range(FREQ_M(880),FREQ_M(915));
        case 9  : return range(FREQ_K(1749900),FREQ_K(1784900));
        case 10 : return range(FREQ_M(1710),FREQ_M(1770));
        case 11 : return range(FREQ_K(1427900),FREQ_K(1447900));
        case 12 : return range(FREQ_M(699),FREQ_M(716));
        case 13 : return range(FREQ_M(777),FREQ_M(787));
        case 14 : return range(FREQ_M(788),FREQ_M(798));
        case 17 : return range(FREQ_M(704),FREQ_M(716));
        case 18 : return range(FREQ_M(815),FREQ_M(830));
        case 19 : return range(FREQ_M(830),FREQ_M(845));
        case 20 : return range(FREQ_M(832),FREQ_M(862));
        case 21 : return range(FREQ_K(1447900),FREQ_K(1462900));
        case 22 : return range(FREQ_M(3410),FREQ_M(3490));
        case 23 : return range(FREQ_M(2000),FREQ_M(2020));
        case 24 : return range(FREQ_K(1626500),FREQ_K(1660500));
        case 25 : return range(FREQ_M(1850),FREQ_M(1915));
        case 26 : return range(FREQ_M(814),FREQ_M(849));
        case 27 : return range(FREQ_M(807),FREQ_M(824));
        case 28 : return range(FREQ_M(703),FREQ_M(748));
        case 29 : return range(Band_NA);
        case 30 : return range(FREQ_M(2305),FREQ_M(2315));
        case 31 : return range(FREQ_K(452500),FREQ_K(457500));
        case 32 : return range(Band_NA);
        case 33 : return range(FREQ_M(1900),FREQ_M(1920));
        case 34 : return range(FREQ_M(2010),FREQ_M(2025));
        case 35 : return range(FREQ_M(1850),FREQ_M(1910));
        case 36 : return range(FREQ_M(1930),FREQ_M(1990));
        case 37 : return range(FREQ_M(1910),FREQ_M(1930));
        case 38 : return range(FREQ_M(2570),FREQ_M(2620));
        case 39 : return range(FREQ_M(1880),FREQ_M(1920));
        case 40 : return range(FREQ_M(2300),FREQ_M(2400));
        case 41 : return range(FREQ_M(2496),FREQ_M(2690));
        case 42 : return range(FREQ_M(3400),FREQ_M(3600));
        case 43 : return range(FREQ_M(3600),FREQ_M(3800));
        case 44 : return range(FREQ_M(703),FREQ_M(803));
        case 45 : return range(FREQ_M(1447),FREQ_M(1467));
        case 46 : return range(FREQ_M(5150),FREQ_M(5925));
        case 47 : return range(FREQ_M(5855),FREQ_M(5925));
        case 48 : return range(FREQ_M(3550),FREQ_M(3700));
        case 49 : return range(FREQ_M(3550),FREQ_M(3700));
        case 50 : return range(FREQ_M(1432),FREQ_M(1517));
        case 51 : return range(FREQ_M(1427),FREQ_M(1432));
        case 52 : return range(FREQ_M(3300),FREQ_M(3400));
        case 53 : return range(FREQ_K(2483500),FREQ_M(2495));
        case 65 : return range(FREQ_M(1920),FREQ_M(2010));
        case 66 : return range(FREQ_M(1710),FREQ_M(1780));
        case 67 : return range(Band_NA);
        case 68 : return range(FREQ_M(698),FREQ_M(728));
        case 69 : return range(Band_NA);
        case 70 : return range(FREQ_M(1695),FREQ_M(1710));
        case 71 : return range(FREQ_M(663),FREQ_M(698));
        case 72 : return range(FREQ_M(451),FREQ_M(456));
        case 73 : return range(FREQ_M(450),FREQ_M(455));
        case 74 : return range(FREQ_M(1427),FREQ_M(1470));
        case 75 : return range(Band_NA);
        case 76 : return range(Band_NA);
        case 85 : return range(FREQ_M(698),FREQ_M(716));
        case 87 : return range(FREQ_M(410),FREQ_M(415));
        case 88 : return range(FREQ_M(412),FREQ_M(417));
        default : return range(Band_Reserved);
        }
    }

    range dl_range() const
    {
        if (duplex_mode() == TDD) {
            return ul_range();
        }

        switch (_band) {
        case 1  : return range(FREQ_M(2110),FREQ_M(2170));
        case 2  : return range(FREQ_M(1930),FREQ_M(1990));
        case 3  : return range(FREQ_M(1805),FREQ_M(1880));
        case 4  : return range(FREQ_M(2110),FREQ_M(2155));
        case 5  : return range(FREQ_M(869),FREQ_M(894));
        case 6  : return range(FREQ_M(875),FREQ_M(885));
        case 7  : return range(FREQ_M(2620),FREQ_M(2690));
        case 8  : return range(FREQ_M(925),FREQ_M(960));
        case 9  : return range(FREQ_K(1844900),FREQ_K(1879900));
        case 10 : return range(FREQ_M(2110),FREQ_M(2170));
        case 11 : return range(FREQ_K(1475900),FREQ_K(1495900));
        case 12 : return range(FREQ_M(729),FREQ_M(746));
        case 13 : return range(FREQ_M(746),FREQ_M(756));
        case 14 : return range(FREQ_M(758),FREQ_M(768));
        case 17 : return range(FREQ_M(734),FREQ_M(746));
        case 18 : return range(FREQ_M(860),FREQ_M(875));
        case 19 : return range(FREQ_M(875),FREQ_M(890));
        case 20 : return range(FREQ_M(791),FREQ_M(821));
        case 21 : return range(FREQ_K(1495900),FREQ_K(1510900));
        case 22 : return range(FREQ_M(3510),FREQ_M(3590));
        case 23 : return range(FREQ_M(2180),FREQ_M(2200));
        case 24 : return range(FREQ_M(1525),FREQ_M(1559));
        case 25 : return range(FREQ_M(1930),FREQ_M(1995));
        case 26 : return range(FREQ_M(859),FREQ_M(894));
        case 27 : return range(FREQ_M(852),FREQ_M(869));
        case 28 : return range(FREQ_M(758),FREQ_M(803));
        case 29 : return range(FREQ_M(717),FREQ_M(728));
        case 30 : return range(FREQ_M(2350),FREQ_M(2360));
        case 31 : return range(FREQ_K(462500),FREQ_K(467500));
        case 32 : return range(FREQ_M(1452),FREQ_M(1496));
        case 65 : return range(FREQ_M(2110),FREQ_M(2200));
        case 66 : return range(FREQ_M(2110),FREQ_M(2200));
        case 67 : return range(FREQ_M(738),FREQ_M(758));
        case 68 : return range(FREQ_M(753),FREQ_M(783));
        case 69 : return range(FREQ_M(2570),FREQ_M(2620));
        case 70 : return range(FREQ_M(1995),FREQ_M(2020));
        case 71 : return range(FREQ_M(617),FREQ_M(652));
        case 72 : return range(FREQ_M(461),FREQ_M(466));
        case 73 : return range(FREQ_M(460),FREQ_M(465));
        case 74 : return range(FREQ_M(1475),FREQ_M(1518));
        case 75 : return range(FREQ_M(1432),FREQ_M(1517));
        case 76 : return range(FREQ_M(1427),FREQ_M(1432));
        case 85 : return range(FREQ_M(728),FREQ_M(746));
        case 87 : return range(FREQ_M(420),FREQ_M(425));
        case 88 : return range(FREQ_M(422),FREQ_M(427));
        default : return range(Band_Reserved);
        }
    }

    duplex_mode_t duplex_mode() const
    { return  (_band < 33 || _band > 64) ? FDD : TDD; }

private:
    uint8_t _band;
};

} // namespace ns_eutra

#endif


