#include "subinfowidget.h"
#include <QFormLayout>
#include <QSpinBox>
#include "uifactory.h"
#include "httpfunc.h"

#include <QJsonObject>
#include <QJsonDocument>

SubInfoWidget::SubInfoWidget(QWidget* parent /*= nullptr*/)
	:QDialog(parent)
{
	setWindowFlags(windowFlags() & ~Qt::WindowContextHelpButtonHint);
	//setAutoFillBackground(true);

	setFixedWidth(240);

	QFormLayout* pFormLayout = new QFormLayout(this);
	//pFormLayout->setLabelAlignment(Qt::AlignRight);
	ControlPorperty property1, property2;

	pFormLayout->addRow(UiFactory::createControl<QLabel>(property1.init().setText("ID:")),
						UiFactory::createControl<QLabel>(property2.init().setObjectname("ID").setText("1")));

	pFormLayout->addRow(UiFactory::createControl<QLabel>(property1.init().setText(tr("Online:"))),
						UiFactory::createControl<QLabel>(property2.init().setObjectname("Online").setText(tr("OnLine")).setReadOnlyState(true)));

	/*pFormLayout->addRow(UiFactory::createControl<QLabel>(property1.init().setText(tr("Fence Type:")),
						UiFactory::createControl<QLabel>(property2.init().setObjectname("FenceType").setText(tr("Dangerous")))));*/

	QSpinBox* pRotateSpinBox = UiFactory::createControl<QSpinBox>(property2.init().setObjectname("RotateSpeed").setMin(0).setMax(100).setValue(50));
	pFormLayout->addRow(UiFactory::createControl<QLabel>(property1.init().setText(tr("Rotate Speed:"))), pRotateSpinBox);

	QHBoxLayout* pHLayout = new QHBoxLayout();
	QPushButton* pOkButton = UiFactory::createControl<QPushButton>(property1.init().setObjectname("Ok").setText(tr("Ok")));
	QPushButton* pLockBtn = UiFactory::createControl<QPushButton>(property2.init().setObjectname("Lock").setText(tr("Close")));
	pHLayout->addWidget(pOkButton);
	pHLayout->addWidget(pLockBtn);
	pFormLayout->addRow(pHLayout);

	connect(pOkButton, &QPushButton::clicked, this, [=](bool check) {

		if (pOkButton->text() == tr("Ok"))
		{
			pOkButton->setText(tr("Edit"));
			// 发送命令给风机，并且设置为不可编辑
			pRotateSpinBox->setEnabled(false);

			HttpFunc::request(QString("http://localhost:8080/set?id=%0&n=%1").arg(getId()).arg(getSpeed()), [](int error,const QString& str)->void {
				// do something or not do at all
			});
		}
		else
		{
			pOkButton->setText(tr("Ok"));
			// 设置为可编辑
			pRotateSpinBox->setEnabled(true);
		}
	});

	connect(pLockBtn, &QPushButton::clicked, this, [=](bool check) {
		/*if (pLockBtn->text() == tr("Lock"))
		{
			pLockBtn->setText(tr("Unlock"));
		}
		else
		{
			pLockBtn->setText(tr("Lock"));
		}*/

		done(0);
	});
}

void SubInfoWidget::setOnline(bool b)
{
	findChild<QLabel*>("Online")->setText(b ? tr("On Line") : tr("Off Line"));
}

void SubInfoWidget::setId(int id)
{
	findChild<QLabel*>("ID")->setText(QString::number(id));
}

void SubInfoWidget::setSpeed(int speed)
{
	findChild<QSpinBox*>("RotateSpeed")->setValue(speed);
}

int SubInfoWidget::getId() const
{
	return findChild<QLabel*>("ID")->text().toInt();
}

int SubInfoWidget::getSpeed() const
{
	return findChild<QSpinBox*>("RotateSpeed")->value();
}
