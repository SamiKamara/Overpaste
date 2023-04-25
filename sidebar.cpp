#include "sidebar.hh"
#include <QFrame>
#include <QLabel>

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

    // Create a layout for the sidebar content
    QVBoxLayout *sidebarContentLayout = new QVBoxLayout(sidebarFrame);
    sidebarContentLayout->setContentsMargins(0, 0, 0, 0);
    sidebarContentLayout->setSpacing(0);

    const QString buttonStyle = "QPushButton { text-align: left; color: #9aa0a6; background-color: #21252b; border: none; padding: 10px; font-size: 14px; font-weight: 700; }" \
        "QPushButton:hover { background-color: #393d47; }";

    // Add buttons to the sidebar
    QIcon menuIcon(":/icons/menu.svg");
    QPixmap menuPixmap = menuIcon.pixmap(QSize(22, 22));
    QPushButton *menuButton = new QPushButton(this);
    menuButton->setIcon(QIcon(menuPixmap)); // Set the icon of the button
    menuButton->setIconSize(menuPixmap.size()); // Set the size of the icon
    menuButton->setText(" Toggle sidebar");
    menuButton->setCursor(Qt::PointingHandCursor);
    menuButton->setStyleSheet(buttonStyle);
    sidebarContentLayout->addWidget(menuButton);

    // Add a label for the categories
    QLabel *filesLabel = new QLabel("Files", this);
    filesLabel->setStyleSheet("color: #9aa0a6; font-size: 10px; font-weight: 700; padding: 10px; text-transform: uppercase; font-weight: 700; margin-bottom: -5px; ");
    sidebarContentLayout->addWidget(filesLabel);

    // Add buttons to the sidebar
    QIcon allIcon(":/icons/all.svg");
    QPixmap allPixmap = allIcon.pixmap(QSize(22, 22));
    QPushButton *allButton = new QPushButton(this);
    allButton->setIcon(QIcon(allPixmap)); // Set the icon of the button
    allButton->setIconSize(allPixmap.size()); // Set the size of the icon
    allButton->setText(" All files");
    allButton->setCursor(Qt::PointingHandCursor);
    allButton->setStyleSheet(buttonStyle);
    sidebarContentLayout->addWidget(allButton);

    // Add a label for the categories
    QLabel *categoriesLabel = new QLabel("Categories", this);
    categoriesLabel->setStyleSheet("color: #9aa0a6; font-size: 10px; font-weight: 700; padding: 10px; text-transform: uppercase; font-weight: 700; margin-bottom: -5px; ");
    sidebarContentLayout->addWidget(categoriesLabel);

    // Add buttons to the sidebar
    QIcon imagesIcon(":/icons/images.svg");
    QPixmap imagesPixmap = imagesIcon.pixmap(QSize(22, 22));
    QPushButton *imagesButton = new QPushButton(this);
    imagesButton->setIcon(QIcon(imagesPixmap)); // Set the icon of the button
    imagesButton->setIconSize(imagesPixmap.size()); // Set the size of the icon
    imagesButton->setText(" Images");
    imagesButton->setCursor(Qt::PointingHandCursor);
    imagesButton->setStyleSheet(buttonStyle);
    sidebarContentLayout->addWidget(imagesButton);

    QIcon gifsIcon(":/icons/gifs.svg");
    QPixmap gifsPixmap = gifsIcon.pixmap(QSize(22, 22));
    QPushButton *gifsButton = new QPushButton(this);
    gifsButton->setIcon(QIcon(gifsPixmap)); // Set the icon of the button
    gifsButton->setIconSize(gifsPixmap.size()); // Set the size of the icon
    gifsButton->setText(" GIFs");
    gifsButton->setCursor(Qt::PointingHandCursor);
    gifsButton->setStyleSheet(buttonStyle);
    sidebarContentLayout->addWidget(gifsButton);

    QIcon textsIcon(":/icons/texts.svg");
    QPixmap textsPixmap = textsIcon.pixmap(QSize(22, 22));
    QPushButton *textsButton = new QPushButton(this);
    textsButton->setIcon(QIcon(textsPixmap)); // Set the icon of the button
    textsButton->setIconSize(textsPixmap.size()); // Set the size of the icon
    textsButton->setText(" Texts");
    textsButton->setCursor(Qt::PointingHandCursor);
    textsButton->setStyleSheet(buttonStyle);
    sidebarContentLayout->addWidget(textsButton);

    QIcon videosIcon(":/icons/videos.svg");
    QPixmap videosPixmap = videosIcon.pixmap(QSize(22, 22));
    QPushButton *videosButton = new QPushButton(this);
    videosButton->setIcon(QIcon(videosPixmap)); // Set the icon of the button
    videosButton->setIconSize(videosPixmap.size()); // Set the size of the icon
    videosButton->setText(" Videos");
    videosButton->setCursor(Qt::PointingHandCursor);
    videosButton->setStyleSheet(buttonStyle);
    sidebarContentLayout->addWidget(videosButton);

    // Add a spacer at the bottom of the button layout to push buttons to the top
    QSpacerItem *spacer = new QSpacerItem(20, 40, QSizePolicy::Minimum, QSizePolicy::Expanding);
    sidebarContentLayout->addItem(spacer);

    // Add the button layout to the sidebar frame
    sidebarLayout->addWidget(sidebarFrame);

    // Set the layout
    setLayout(sidebarLayout);
}
