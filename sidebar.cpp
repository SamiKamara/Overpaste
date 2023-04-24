#include "sidebar.hh"
#include <QFrame>

Sidebar::Sidebar(QWidget *parent)
    : QWidget(parent)
{
    sidebarLayout = new QVBoxLayout(this);

    // Set margin to 0
    sidebarLayout->setContentsMargins(0, 0, 0, 0);

    // Width of the sidebar
    setFixedWidth(250);

    // Create a frame for the sidebar
    QFrame *sidebarFrame = new QFrame(this);
    sidebarFrame->setStyleSheet("background-color: #21252b;");

    // Create a layout for the buttons
    QVBoxLayout *buttonLayout = new QVBoxLayout(sidebarFrame);
    buttonLayout->setContentsMargins(0, 0, 0, 0);
    buttonLayout->setSpacing(0);

    // Add buttons to the sidebar
    QIcon imagesIcon(":/icons/images.svg");
    QPushButton *imagesButton = new QPushButton(imagesIcon, "Images", this);
    imagesButton->setStyleSheet("text-align: left; color: #fff; background-color: #21252b; border: none; padding: 10px;");
    buttonLayout->addWidget(imagesButton);

    QIcon gifsIcon(":/icons/images.svg");
    QPushButton *gifsButton = new QPushButton(gifsIcon, "GIFs", this);
    gifsButton->setStyleSheet("text-align: left; color: #fff; background-color: #21252b; border: none; padding: 10px;");
    buttonLayout->addWidget(gifsButton);

    QIcon textsIcon(":/icons/images.svg");
    QPushButton *textsButton = new QPushButton(textsIcon, "Texts", this);
    textsButton->setStyleSheet("text-align: left; color: #fff; background-color: #21252b; border: none; padding: 10px;");
    buttonLayout->addWidget(textsButton);

    QIcon videosIcon(":/icons/images.svg");
    QPushButton *videosButton = new QPushButton(videosIcon, "Videos", this);
    videosButton->setStyleSheet("text-align: left; color: #fff; background-color: #21252b; border: none; padding: 10px;");
    buttonLayout->addWidget(videosButton);

    // Add a spacer at the bottom of the button layout to push buttons to the top
    QSpacerItem *spacer = new QSpacerItem(20, 40, QSizePolicy::Minimum, QSizePolicy::Expanding);
    buttonLayout->addItem(spacer);

    // Add the button layout to the sidebar frame
    sidebarLayout->addWidget(sidebarFrame);

    // Set the layout
    setLayout(sidebarLayout);
}
