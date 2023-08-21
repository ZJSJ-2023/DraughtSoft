#include "createfencedlg.h"
#include <QFormLayout>
#include "uifactory.h"
#include <QMessageBox>

CreateFenceDlg::CreateFenceDlg(QWidget* parent /*= nullptr*/)
	:QDialog(parent)
{
	setWindowFlags(windowFlags() & ~Qt::WindowContextHelpButtonHint);

	QFormLayout* pLayout = new QFormLayout(this);

	ControlPorperty property1, property2;
	pLayout->addRow(UiFactory::createControl<QLabel>(property1.init().setText(tr("Name:"))),
					UiFactory::createControl<QLineEdit>(property2.init().setObjectname("Name")));

	pLayout->addRow(UiFactory::createControl<QLabel>(property1.init().setText("Type:")),
					UiFactory::createControl<QComboBox>(property2.init().setObjectname("Type").setTexts({ tr("Dangerous Area"),
																										  tr("Warning Area") ,
																										  tr("Notify Area") })));
	pLayout->addRow(UiFactory::createControl<QLabel>(property1.init().setText("Tag Group:")),
					UiFactory::createControl<QComboBox>(property2.init().setObjectname("Tag").setTexts({ tr("Green Group"),
																										 tr("Orange Group") ,
																										 tr("Yellow Group"),
																										 tr("Blue Group") })));

	QPushButton* pOkButton = UiFactory::createControl<QPushButton>(property1.init().setObjectname("Ok").setText("Ok"));
	QPushButton* pCancelBtn = UiFactory::createControl<QPushButton>(property2.init().setObjectname("Cancel").setText("Cancel"));

	QHBoxLayout* pHLayout = new QHBoxLayout();
	pHLayout->addWidget(pOkButton);
	pHLayout->addWidget(pCancelBtn);
	pLayout->addRow(pHLayout);

	connect(pOkButton, &QPushButton::clicked, this, [=]() {

		if (getName().isEmpty())
		{
			QMessageBox::warning(this, "Warning", "name is empty,please input!", QMessageBox::Ok);
			return;
		}

		done(1);
	});

	connect(pCancelBtn, &QPushButton::clicked, this, [=]() {
		done(0);
	});
}

QString CreateFenceDlg::getName() const
{
	return findChild<QLineEdit*>("Name")->text();
}

QString CreateFenceDlg::getType() const
{
	return findChild<QComboBox*>("Type")->currentText();
}

int CreateFenceDlg::getTag() const
{
	return findChild<QComboBox*>("Tag")->currentIndex();
}
