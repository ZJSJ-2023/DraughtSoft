#include "uifactory.h"
#include "appsettings.h"

#include <QListView.h>
#include <QSpinBox>

#define STANDARD_HEIGHT 30

int textWidth(const QString& text, const QFont& font)
{
	QFontMetrics metrics(font);
	return metrics.horizontalAdvance(text);
}

int maxWidth(const QStringList& texts, const QFont& font)
{
	if (texts.isEmpty())
		return 0;

	QFontMetrics metrics(font);

	int max = metrics.horizontalAdvance(texts[0]);

	for (auto text : texts)
		max = std::max(max, metrics.horizontalAdvance(text));

	return max;
}

QString converRGB16HexStr(QColor color)
{
	QString redStr = QString("%1").arg(color.red(), 2, 16, QChar('0'));
	QString greenStr = QString("%1").arg(color.green(), 2, 16, QChar('0'));
	QString blueStr = QString("%1").arg(color.blue(), 2, 16, QChar('0'));
	QString hexStr = "#" + redStr + greenStr + blueStr;
	return hexStr;
}

namespace UiFactory
{
	QWidget* createComboBox(const ControlPorperty& property, QWidget* parent)
	{
		const int height = 28;

		QComboBox* pCombox = new QComboBox(parent);
		QListView* pListView = new QListView(pCombox);
		pListView->setFont(Appsettings::getAppFont());
		pCombox->setView(pListView);
		pCombox->setFont(Appsettings::getAppFont());
		pCombox->setFixedHeight(height);
		pCombox->setFixedWidth(maxWidth(property.getTexts(), pCombox->font()) + 48);

		QStringList texts = property.getTexts();
		QStringList icons = property.getIcons();

		if (icons.isEmpty())
			pCombox->addItems(texts);
		else if (icons.size() == texts.size())
		{
			/*for (int i = 0; i < texts.size(); ++i)
				pCombox->addItem(PixmapLoaderInstance()->loadPixmap(icons[i], QSize(16, 16)), texts[i]);*/
		}

		pCombox->setStyleSheet(QString("QComboBox QAbstractItemView::item {height:%0px;}").arg(height));

		return pCombox;
	}

	QWidget* createPushButton(const ControlPorperty& property, QWidget* parent /*= nullptr*/)
	{
		QString iconpath = property.getIcon();

		QPushButton* pBtn = new QPushButton(parent);
		if (iconpath.isEmpty())
		{
			pBtn->setText(property.getText());
			pBtn->setFont(Appsettings::getAppFont());
			pBtn->setFixedHeight(STANDARD_HEIGHT);
			//pBtn->setFixedWidth(textWidth(text, pBtn->font()) + 20);
		}
		else
		{
			pBtn->setFixedSize(property.getSize());
			pBtn->setIconSize(property.getSize());
			//pBtn->setIcon(QIcon(PixmapLoaderInstance()->loadPixmap(iconpath, property.getSize())));
			pBtn->setStyleSheet("border:none;");
		}

		pBtn->setCheckable(property.getCheckableState());

		return pBtn;
	}

	QWidget* createLabel(const ControlPorperty& property, QWidget* parent /*= nullptr*/)
	{
		const QString stylesheet = property.getStylesheet();

		QLabel* pLabel = new QLabel(parent);

		if (property.getIcon().isEmpty())
		{
			pLabel->setText(property.getText());
			pLabel->setFont(Appsettings::getAppFont());
			pLabel->setFixedHeight(STANDARD_HEIGHT);
			pLabel->setStyleSheet(stylesheet.isEmpty() ?
								  QString("color:%0;").arg(converRGB16HexStr(property.getColor())) : stylesheet);
		}
		else
		{
			//pLabel->setPixmap(PixmapLoaderInstance()->loadPixmap(property.getIcon(), property.getSize()));
		}

		return pLabel;
	}

	QWidget* createLineEdit(const ControlPorperty& property, QWidget* parent /*= nullptr*/)
	{
		QLineEdit* pLineEdit = new QLineEdit(parent);
		pLineEdit->setText(property.getText());
		pLineEdit->setFont(Appsettings::getAppFont());
		pLineEdit->setFixedHeight(STANDARD_HEIGHT);
		pLineEdit->setReadOnly(property.getReadOnlyState());

		return pLineEdit;
	}

	QWidget* createSlider(const ControlPorperty& property, QWidget* parent)
	{
		QSlider* pSlider = new QSlider(property.getOrientation(), parent);
		pSlider->setFont(Appsettings::getAppFont());
		pSlider->setFixedHeight(STANDARD_HEIGHT);
		pSlider->setMinimum(property.getMin());
		pSlider->setMaximum(property.getMax());

		return pSlider;
	}

	QWidget* createCheckBox(const ControlPorperty& property, QWidget* parent)
	{
		QCheckBox* pCheckBox = new QCheckBox(parent);
		pCheckBox->setText(property.getText());
		pCheckBox->setFont(Appsettings::getAppFont());
		//pChechBox->setFixedHeight(STANDARD_HEIGHT);

		return pCheckBox;
	}

	QWidget* createSpinBox(const ControlPorperty& property, QWidget* parent /*= nullptr*/)
	{
		QSpinBox* pSpinBox = new QSpinBox(parent);
		pSpinBox->setValue(property.getValue());
		pSpinBox->setMinimum(property.getMin());
		pSpinBox->setMaximum(property.getMax());
		pSpinBox->setFont(Appsettings::getAppFont());
		pSpinBox->setFixedHeight(STANDARD_HEIGHT);

		return pSpinBox;
	}

};