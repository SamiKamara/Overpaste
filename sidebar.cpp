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
    sidebarContentLayout->addWidget(createButton(" Toggle sidebar", QIcon(":/icons/menu.svg"), buttonStyle));

    // Add a label for the categories
    QLabel *filesLabel = new QLabel("Files", this);
    filesLabel->setStyleSheet("color: #9aa0a6; font-size: 10px; font-weight: 700; padding: 10px; text-transform: uppercase; font-weight: 700; margin-bottom: -5px; ");
    sidebarContentLayout->addWidget(filesLabel);

    // Add buttons to the sidebar
    sidebarContentLayout->addWidget(createButton(" All files", QIcon(":/icons/all.svg"), buttonStyle));

    // Add a label for the categories
    QLabel *categoriesLabel = new QLabel("Categories", this);
    categoriesLabel->setStyleSheet("color: #9aa0a6; font-size: 10px; font-weight: 700; padding: 10px; text-transform: uppercase; font-weight: 700; margin-bottom: -5px; ");
    sidebarContentLayout->addWidget(categoriesLabel);

    // Add buttons to the sidebar
    sidebarContentLayout->addWidget(createButton(" Images", QIcon(":/icons/images.svg"), buttonStyle));
    sidebarContentLayout->addWidget(createButton(" GIFs", QIcon(":/icons/gifs.svg"), buttonStyle));
    sidebarContentLayout->addWidget(createButton(" Texts", QIcon(":/icons/texts.svg"), buttonStyle));
    sidebarContentLayout->addWidget(createButton(" Videos", QIcon(":/icons/videos.svg"), buttonStyle));

    // Add a spacer at the bottom of the button layout to push buttons to the top
    QSpacerItem *spacer = new QSpacerItem(20, 40, QSizePolicy::Minimum, QSizePolicy::Expanding);
    sidebarContentLayout->addItem(spacer);

    // Add the button layout to the sidebar frame
    sidebarLayout->addWidget(sidebarFrame);
}

QPushButton* Sidebar::createButton(const QString& text, const QIcon& icon, const QString& style)
{
    QPushButton *button = new QPushButton(this);
    button->setIcon(icon);
    button->setIconSize(QSize(22, 22));
    button->setText(text);
    button->setStyleSheet(style);
    button->setFixedSize(250, 40);
    button->setCursor(Qt::PointingHandCursor);
    return button;
}
