#ifndef UES_GUI_SIMULATION_PROGRESS_H
#define UES_GUI_SIMULATION_PROGRESS_H

#include <QWidget>

#include <sim/simulation.h>

namespace Ui
{
class simulation_progress;
}

namespace ues
{
namespace gui
{


class simulation_progress : public QWidget
{
    Q_OBJECT
public:
    simulation_progress ( ues::sim::simulation & sim, QWidget * parent = nullptr );
    simulation_progress ( const simulation_progress & ) = delete;
    ~simulation_progress();

    simulation_progress & operator= ( const simulation_progress & ) = delete;

public slots:
    void update_progress ( unsigned int );

signals:
    void progress_changed ( unsigned int );
    void simulation_complete();

private:
    Ui::simulation_progress * ui;
    unsigned int simulation_size;
};
}
}

#endif // UES_GUI_SIMULATION_PROGRESS_H
