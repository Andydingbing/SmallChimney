#ifndef STARPOINT_SP9500_CHILD_WIDGET_H
#define STARPOINT_SP9500_CHILD_WIDGET_H

namespace ns_starpoint {
namespace ns_sp9500 {

class ChildWidgets : public ChildWidgetHelper
{
    Q_OBJECT

public:
    ChildWidgets(MainWindow *parent);

    void init();
    void initMenu();

    void updatePtr();

    void mainTabCurrentChanged(int index);

public slots:
    void initChildWidgets();

    void selSP3301_0();

    void selSP3301_1();

    void selSP3301_2();

    void selSP3301_3();

    void selSP3301_4();

private:
    QMenu *menuDevice;
    QAction *actionInit;
    QAction *actionPCIE_Restart;
    QAction *actionTuneOCXO;
    QAction *actionExit;

    QMenu *menuRFU;
    QAction *actionRFU0;
    QAction *actionRFU1;
    QAction *actionRFU2;
    QAction *actionRFU3;
    QAction *actionRFU4;

    QMenu *menuFunction;
    QAction *actionArbViewer;
    QAction *actionStarAllIQ_Capture;
    QAction *actionStopAllIQ_Capture;
    QAction *actionLogConfig;
    QAction *actionLogViewer;

    QMenu *menuTest;
    QAction *actionProgramFPGAbit;
    QAction *actionAlgorithm_5355;
    QAction *actionFittingInterpolation;

    QMenu *menuHelp;
    QAction *actionAbout;
};

} // namespace ns_sp9500
} // namespace ns_starpoint

#endif
