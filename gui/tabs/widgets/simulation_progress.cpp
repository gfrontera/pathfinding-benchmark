#include "simulation_progress.h"
#include "ui_simulation_progress.h"

using namespace ues::gui;


simulation_progress::simulation_progress ( ues::sim::simulation & sim, QWidget * parent )
    : QWidget ( parent ),
      ui ( new Ui::simulation_progress ),
      simulation_size ( sim.get_size() )
{
    ui->setupUi ( this );

    ui->progress_bar->setMaximum ( simulation_size );
    ui->progress_bar->setValue ( 0 );

    // Bind simulation progress to changes in the interface.
    sim.set_on_progress_update ( [this] ( unsigned int value ) { emit progress_changed ( value ); } );
    connect ( this, SIGNAL ( progress_changed ( unsigned int ) ), this, SLOT ( update_progress ( unsigned int ) ) );
}


simulation_progress::~simulation_progress()
{
    delete ui;
}


void simulation_progress::update_progress ( unsigned int value )
{
    ui->progress_bar->setValue ( value );

    if ( value == simulation_size )
    {
        emit simulation_complete();
    }
}
