#include "ListaNotificareReadOnlyGUI.h"

#include "style.h"
#include <QPainter>
#include <QRandomGenerator>
#include <algorithm>

ListaNotificareReadOnlyGUI::ListaNotificareReadOnlyGUI(ServiceLocatar& service, QWidget* parent)
    : QWidget(parent), service(service)
{
    setWindowTitle("Lista notificare desenata");
    resize(700, 500);
    setStyleSheet(appStyle());

    service.addListaNotificareObserver(this);
    regenereazaFiguri();
}

ListaNotificareReadOnlyGUI::~ListaNotificareReadOnlyGUI()
{
    service.removeListaNotificareObserver(this);
}

void ListaNotificareReadOnlyGUI::regenereazaFiguri()
{
    const auto count = static_cast<int>(service.getListaNotificareSize());
    pozitii.clear();
    dimensiuni.clear();
    pozitii.reserve(static_cast<std::size_t>(count));
    dimensiuni.reserve(static_cast<std::size_t>(count));

    const int left = 20;
    const int top = 55;
    const int right = width() - 20;
    const int bottom = height() - 20;

    for (int i = 0; i < count; ++i)
    {
        const int dim = QRandomGenerator::global()->bounded(18, 56);
        const int xMin = left;
        const int xMax = std::max(left + 1, right - dim);
        const int yMin = top;
        const int yMax = std::max(top + 1, bottom - dim);
        const int x = QRandomGenerator::global()->bounded(xMin, xMax);
        const int y = QRandomGenerator::global()->bounded(yMin, yMax);
        pozitii.emplace_back(x, y);
        dimensiuni.push_back(dim);
    }
}

void ListaNotificareReadOnlyGUI::update()
{
    regenereazaFiguri();
    QWidget::update();
}

void ListaNotificareReadOnlyGUI::paintEvent(QPaintEvent* ev)
{
    QWidget::paintEvent(ev);

    QPainter painter(this);
    painter.setRenderHint(QPainter::Antialiasing, true);
    painter.fillRect(rect(), QColor(15, 23, 42));

    const int count = static_cast<int>(service.getListaNotificareSize());
    painter.setPen(QColor(229, 231, 235));
    painter.drawText(16, 32, QString("Elemente in lista de notificare: %1").arg(count));

    for (int i = 0; i < count && i < static_cast<int>(pozitii.size()) && i < static_cast<int>(dimensiuni.size()); ++i)
    {
        const QPoint& p = pozitii[static_cast<std::size_t>(i)];
        const int dim = dimensiuni[static_cast<std::size_t>(i)];

        if (i % 3 == 0)
        {
            painter.setBrush(QColor(37, 99, 235, 190));
            painter.setPen(QColor(147, 197, 253));
            painter.drawEllipse(p.x(), p.y(), dim, dim);
        }
        else if (i % 3 == 1)
        {
            painter.setBrush(QColor(5, 150, 105, 190));
            painter.setPen(QColor(110, 231, 183));
            painter.drawRect(p.x(), p.y(), dim, dim);
        }
        else
        {
            painter.setBrush(QColor(217, 119, 6, 190));
            painter.setPen(QColor(251, 191, 36));
            painter.drawEllipse(p.x(), p.y(), dim + 6, dim - 4);
        }
    }
}

void ListaNotificareReadOnlyGUI::resizeEvent(QResizeEvent* ev)
{
    QWidget::resizeEvent(ev);
    regenereazaFiguri();
}
