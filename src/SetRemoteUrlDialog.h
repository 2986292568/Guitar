#ifndef SETREMOTEURLDIALOG_H
#define SETREMOTEURLDIALOG_H

#include <QDialog>
#include "Git.h"
#include "RepositoryPropertyDialog.h"

class MainWindow;

namespace Ui {
class SetRemoteUrlDialog;
}

class SetRemoteUrlDialog : public BasicRepositoryDialog
{
	Q_OBJECT
private:
	QStringList remotes;
public:
	explicit SetRemoteUrlDialog(MainWindow *mainwindow, QStringList const &remotes, GitPtr g);
	~SetRemoteUrlDialog();

	int exec();
private:
	Ui::SetRemoteUrlDialog *ui;
	void updateRemotesTable();
public slots:
	void accept();
private slots:
	void on_pushButton_test_clicked();
};

#endif // SETREMOTEURLDIALOG_H
