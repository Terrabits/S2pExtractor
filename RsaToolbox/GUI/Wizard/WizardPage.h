#ifndef WIZARDPAGE_H
#define WIZARDPAGE_H


#include "Wizard.h"
#include "WizardButtons.h"
#include "BreadCrumbs.h"

#include <QWidget>
#include <QSharedPointer>
#include <QList>


namespace RsaToolbox {

class Wizard;
class BreadCrumbs;
class WizardButtons;
class WizardPage;
typedef QList<WizardPage*> WizardPages;

class WizardPage : public QWidget
{
    Q_OBJECT
    friend class Wizard;

public:
    explicit WizardPage(QWidget *parent = 0);
    ~WizardPage();

    QString name() const;
    void setName(QString name);
    int nextIndex() const;
    void setNextIndex(int index);
    bool isFinalPage() const;
    void setFinalPage(bool isFinalPage);

    // Normal Navigation
    virtual void initialize();
    virtual bool skip();
    virtual bool isReadyForNext();
    virtual int next();

    // Navigation Contingencies
    virtual bool isReadyForBack();
    virtual void back();
    virtual bool skipBackwards() const;
    virtual void backToThis();
    virtual void resetContents();

signals:
    void enableNext(bool isEnabled);
    void enableBack(bool isEnabled);

public slots:
    virtual void displayError(QString message);

protected:
    WizardPage *pageAt(int index);
    Wizard *wizard() const;
    virtual void setWizard(Wizard *wizard);
    WizardButtons *buttons() const;
    void clearHistory();
    BreadCrumbs *breadCrumbs() const;

private:
    QString _name;
    bool _isFinal;
    int _next;
    Wizard *_wizard;
};
} // RsaToolbox


#endif // WIZARDPAGE_H
