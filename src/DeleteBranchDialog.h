#ifndef DELETEBRANCHDIALOG_H
#define DELETEBRANCHDIALOG_H

#include <QDialog>

namespace Ui {
class DeleteBranchDialog;
}

class DeleteBranchDialog : public QDialog
{
	Q_OBJECT
private:
	struct Private;
	Private *m;
public:
	explicit DeleteBranchDialog(QWidget *parent, bool remote, const QStringList &all_local_branch_names, const QStringList &current_local_branch_names);
	~DeleteBranchDialog();

	QStringList selectedBranchNames() const;
private slots:
	void on_checkBox_all_branches_toggled(bool checked);

private:
	Ui::DeleteBranchDialog *ui;
	void updateList();
	bool isRemote() const;
};

#endif // DELETEBRANCHDIALOG_H
