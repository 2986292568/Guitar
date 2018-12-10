#include "SettingsDialog.h"
#include "ui_SettingsDialog.h"
#include "MySettings.h"

#include <QFileDialog>
#include "common/misc.h"

static int page_number = 0;


SettingsDialog::SettingsDialog(MainWindow *parent) :
	QDialog(parent),
	ui(new Ui::SettingsDialog)
{
	ui->setupUi(this);
	Qt::WindowFlags flags = windowFlags();
	flags &= ~Qt::WindowContextHelpButtonHint;
	setWindowFlags(flags);

	mainwindow_ = parent;

	loadSettings();

	QTreeWidgetItem *item;

	auto AddPage = [&](QWidget *page){
		page->layout()->setMargin(0);
		QString name = page->windowTitle();
		item = new QTreeWidgetItem();
		item->setText(0, name);
		item->setData(0, Qt::UserRole, QVariant::fromValue((uintptr_t)(QWidget *)page));
		ui->treeWidget->addTopLevelItem(item);
	};
	AddPage(ui->page_general);
	AddPage(ui->page_programs);
	AddPage(ui->page_behavior);
	AddPage(ui->page_network);
	AddPage(ui->page_example);

	ui->treeWidget->setCurrentItem(ui->treeWidget->topLevelItem(page_number));
}

SettingsDialog::~SettingsDialog()
{
	delete ui;
}

void SettingsDialog::loadSettings(ApplicationSettings *as)
{
	MySettings s;

	ApplicationSettings def = ApplicationSettings::defaultSettings();

	auto STRING_VALUE_ = [&](QString const &name, QString *v, QString const &def){
		*v = s.value(name, def).toString();
	};

	auto BOOL_VALUE_ = [&](QString const &name, bool *v, bool const &def){
		*v = s.value(name, def).toBool();
	};

	auto INT_VALUE_ = [&](QString const &name, int *v, int const &def){
		*v = s.value(name, def).toInt();
	};
	(void)INT_VALUE_;

	auto UINT_VALUE_ = [&](QString const &name, unsigned int *v, unsigned int const &def){
		*v = s.value(name, def).toUInt();
	};

#define STRING_VALUE(NAME, SYMBOL) STRING_VALUE_(NAME, &as->SYMBOL, def.SYMBOL)
#define BOOL_VALUE(NAME, SYMBOL)   BOOL_VALUE_(NAME, &as->SYMBOL, def.SYMBOL)
#define INT_VALUE(NAME, SYMBOL)   INT_VALUE_(NAME, &as->SYMBOL, def.SYMBOL)
#define UINT_VALUE(NAME, SYMBOL)   UINT_VALUE_(NAME, &as->SYMBOL, def.SYMBOL)

	s.beginGroup("Global");
	BOOL_VALUE("SaveWindowPosition", remember_and_restore_window_position);
	STRING_VALUE("DefaultWorkingDirectory", default_working_dir);
	STRING_VALUE("GitCommand", git_command);
	STRING_VALUE("FileCommand", file_command);
	STRING_VALUE("GpgCommand", gpg_command);
	s.endGroup();

	s.beginGroup("UI");
	BOOL_VALUE("EnableHighDpiScaling", enable_high_dpi_scaling);
	s.endGroup();

	s.beginGroup("Network");
	STRING_VALUE("ProxyType", proxy_type);
	STRING_VALUE("ProxyServer", proxy_server);
	as->proxy_server = misc::makeProxyServerURL(as->proxy_server);
	BOOL_VALUE("GetCommitterIcon", get_committer_icon);
	s.endGroup();

	s.beginGroup("Behavior");
	BOOL_VALUE("AutomaticFetch", automatically_fetch_when_opening_the_repository);
	UINT_VALUE("WatchRemoteInterval", watch_remote_changes_every_mins);
	UINT_VALUE("MaxCommitItemAcquisitions", maximum_number_of_commit_item_acquisitions);
	s.endGroup();
}

void SettingsDialog::saveSettings(ApplicationSettings const *as)
{
	MySettings s;

	s.beginGroup("Global");
	s.setValue("SaveWindowPosition", as->remember_and_restore_window_position);
	s.setValue("DefaultWorkingDirectory", as->default_working_dir);
	s.setValue("GitCommand", as->git_command);
	s.setValue("FileCommand", as->file_command);
	s.setValue("GpgCommand", as->gpg_command);
	s.endGroup();

	s.beginGroup("UI");
	s.setValue("EnableHighDpiScaling", as->enable_high_dpi_scaling);
	s.endGroup();

	s.beginGroup("Network");
	s.setValue("ProxyType", as->proxy_type);
	s.setValue("ProxyServer", misc::makeProxyServerURL(as->proxy_server));
	s.setValue("GetCommitterIcon", as->get_committer_icon);
	s.endGroup();

	s.beginGroup("Behavior");
	s.setValue("AutomaticFetch", as->automatically_fetch_when_opening_the_repository);
	s.setValue("WatchRemoteInterval", as->watch_remote_changes_every_mins);
	s.setValue("MaxCommitItemAcquisitions", as->maximum_number_of_commit_item_acquisitions);
	s.endGroup();

}

void SettingsDialog::saveSettings()
{
	saveSettings(&set);
}

void SettingsDialog::exchange(bool save)
{
	QList<AbstractSettingForm *> forms = ui->stackedWidget->findChildren<AbstractSettingForm *>();
	for (AbstractSettingForm *form : forms) {
		form->exchange(save);
	}
}

void SettingsDialog::loadSettings()
{
	loadSettings(&set);
	exchange(false);
}

void SettingsDialog::done(int r)
{
	page_number = ui->treeWidget->currentIndex().row();
	QDialog::done(r);
}

void SettingsDialog::accept()
{
	exchange(true);
	saveSettings();
	done(QDialog::Accepted);
}

void SettingsDialog::on_treeWidget_currentItemChanged(QTreeWidgetItem *current, QTreeWidgetItem *previous)
{
	(void)previous;
	if (current) {
		uintptr_t p = current->data(0, Qt::UserRole).value<uintptr_t>();
		QWidget *w = reinterpret_cast<QWidget *>(p);
		Q_ASSERT(w);
		ui->stackedWidget->setCurrentWidget(w);
	}
}

