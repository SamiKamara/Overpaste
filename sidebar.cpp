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

    const QString buttonStyle = "QPushButton { text-align: left; color: #fff; background-color: #21252b; border: none; padding: 10px; font-size: 14px; font-weight: 700; }" \
        "QPushButton:hover { background-color: #393d47; }";

    // Add buttons to the sidebar
    QIcon imagesIcon(":/icons/images.svg");
    QPixmap imagesPixmap = imagesIcon.pixmap(QSize(22, 22));
    QPushButton *imagesButton = new QPushButton(this);
    imagesButton->setIcon(QIcon(imagesPixmap)); // Set the icon of the button
    imagesButton->setIconSize(imagesPixmap.size()); // Set the size of the icon
    imagesButton->setText("Images");
    imagesButton->setCursor(Qt::PointingHandCursor);
    imagesButton->setStyleSheet(buttonStyle);
    buttonLayout->addWidget(imagesButton);


    QIcon gifsIcon(":/icons/images.svg");
    QPixmap gifsPixmap = gifsIcon.pixmap(QSize(22, 22));
    QPushButton *gifsButton = new QPushButton(this);
    gifsButton->setIcon(QIcon(gifsPixmap)); // Set the icon of the button
    gifsButton->setIconSize(gifsPixmap.size()); // Set the size of the icon
    gifsButton->setText("GIFs");
    gifsButton->setCursor(Qt::PointingHandCursor);
    gifsButton->setStyleSheet(buttonStyle);
    buttonLayout->addWidget(gifsButton);
    

    QIcon textsIcon(":/icons/images.svg");
    QPixmap textsPixmap = textsIcon.pixmap(QSize(22, 22));
    QPushButton *textsButton = new QPushButton(this);
    textsButton->setIcon(QIcon(textsPixmap)); // Set the icon of the button
    textsButton->setIconSize(textsPixmap.size()); // Set the size of the icon
    textsButton->setText("Texts");
    textsButton->setCursor(Qt::PointingHandCursor);
    textsButton->setStyleSheet(buttonStyle);
    buttonLayout->addWidget(textsButton);

    QIcon videosIcon(":/icons/images.svg");
    QPixmap videosPixmap = videosIcon.pixmap(QSize(22, 22));
    QPushButton *videosButton = new QPushButton(this);
    videosButton->setIcon(QIcon(videosPixmap)); // Set the icon of the button
    videosButton->setIconSize(videosPixmap.size()); // Set the size of the icon
    videosButton->setText("Videos");
    videosButton->setCursor(Qt::PointingHandCursor);
    videosButton->setStyleSheet(buttonStyle);
    buttonLayout->addWidget(videosButton);

    // Add a spacer at the bottom of the button layout to push buttons to the top
    QSpacerItem *spacer = new QSpacerItem(20, 40, QSizePolicy::Minimum, QSizePolicy::Expanding);
    buttonLayout->addItem(spacer);

    // Add the button layout to the sidebar frame
    sidebarLayout->addWidget(sidebarFrame);

    // Set the layout
    setLayout(sidebarLayout);
}
