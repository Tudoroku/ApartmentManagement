#ifndef LAB6_7_STYLE_H
#define LAB6_7_STYLE_H

#include <QString>

inline QString appStyle()
{
    return R"(
        * {
            font-family: "Segoe UI", "Inter", "Arial";
            font-size: 14px;
        }

        QWidget {
            background: #0F172A;
            color: #E5E7EB;
        }

        QLabel#Title {
            font-size: 28px;
            font-weight: 800;
            color: #F8FAFC;
        }

        QLabel#Subtitle {
            font-size: 13px;
            color: #94A3B8;
        }

        QLabel#SectionTitle {
            font-size: 16px;
            font-weight: 700;
            color: #F8FAFC;
        }

        QFrame#Card {
            background: #111827;
            border: 1px solid #1F2937;
            border-radius: 18px;
        }

        QLineEdit {
            background: #0B1220;
            border: 1px solid #334155;
            border-radius: 10px;
            padding: 10px 12px;
            color: #F8FAFC;
            selection-background-color: #2563EB;
        }

        QLineEdit:focus {
            border: 1px solid #60A5FA;
        }

        QPushButton {
            background: #1D4ED8;
            border: none;
            border-radius: 10px;
            padding: 10px 14px;
            color: #F8FAFC;
            font-weight: 700;
        }

        QPushButton:hover {
            background: #2563EB;
        }

        QPushButton:pressed {
            background: #1E40AF;
        }

        QPushButton#SecondaryButton {
            background: #1F2937;
            border: 1px solid #334155;
        }

        QPushButton#SecondaryButton:hover {
            background: #334155;
        }

        QPushButton#DangerButton {
            background: #B91C1C;
        }

        QPushButton#DangerButton:hover {
            background: #DC2626;
        }

        QTableWidget {
            background: #111827;
            alternate-background-color: #0B1220;
            border: none;
            border-radius: 14px;
            gridline-color: #1F2937;
            selection-background-color: #2563EB;
            selection-color: #F8FAFC;
        }

        QTableWidget::item {
            padding: 8px;
            border: none;
        }

        QHeaderView::section {
            background: #0B1220;
            color: #CBD5E1;
            padding: 10px;
            border: none;
            font-weight: 800;
        }

        QScrollBar:vertical {
            background: #0F172A;
            width: 12px;
            margin: 0;
        }

        QScrollBar::handle:vertical {
            background: #334155;
            border-radius: 6px;
            min-height: 30px;
        }
    )";
}

#endif
