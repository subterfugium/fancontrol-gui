/*
 * <one line to give the library's name and an idea of what it does.>
 * Copyright 2015  <copyright holder> <email>
 * 
 * This program is free software; you can redistribute it and/or
 * modify it under the terms of the GNU General Public License as
 * published by the Free Software Foundation; either version 2 of
 * the License or (at your option) version 3 or any later version
 * accepted by the membership of KDE e.V. (or its successor approved
 * by the membership of KDE e.V.), which shall act as a proxy
 * defined in Section 14 of version 3 of the license.
 * 
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 * 
 * You should have received a copy of the GNU General Public License
 * along with this program.  If not, see <http://www.gnu.org/licenses/>.
 * 
 */

#include "fancontrolkcm.h"

#include <QtQml>
#include <KAboutData>
#include <KLocalizedString>
#include <KPluginFactory>

#include "../../lib/src/hwmon.h"
#include "../../lib/src/sensors.h"


K_PLUGIN_FACTORY_WITH_JSON(FancontrolKCMFactory, "kcm_fancontrol.json", registerPlugin<FancontrolKCM>();)

FancontrolKCM::FancontrolKCM(QObject *parent, const QVariantList& args) : ConfigModule(parent, args)
{
    KLocalizedString::setApplicationDomain("fancontrol-gui");

    KAboutData *about = new KAboutData("kcm_fancontrol",
                                       i18n("Fancontrol-KCM"),
                                       "0.1",
                                       i18n("KDE Fancontrol Module"),
                                       KAboutLicense::KAboutLicense::GPL_V2,
                                       "Copyright (C) 2015 Malte Veerman",
                                       QString(),
                                       "http://github.com/maldela/fancontrol-gui",
                                       "http://github.com/maldela/fancontrol-gui/issues");
    about->addAuthor(i18n("Malte Veerman"), i18n("Main Developer"), "maldela@halloarsch.de"); 
    setAboutData(about);
    
    setButtons(FancontrolKCM::Apply);
    setAuthActionName("fancontrol.gui.helper.action");
    
    connect(m_loader, SIGNAL(configFileChanged()), this, SLOT(setNeedsSave()));
    
    qmlRegisterType<Loader>();
    qmlRegisterType<Hwmon>();
    qmlRegisterType<Fan>();
    qmlRegisterType<PwmFan>();
    qmlRegisterType<Temp>();
    
#ifndef NO_SYSTEMD
    qmlRegisterType<SystemdCommunicator>();
#endif
}

void FancontrolKCM::save()
{
    qDebug() << "saving...";
    setNeedsSave(false);
    m_loader->save();
    
#ifndef NO_SYSTEMD
    m_communicator->restartService();
#endif
}

void FancontrolKCM::load()
{
    setNeedsSave(false);
    m_loader->load(QUrl::fromLocalFile("/etc/fancontrol"));
}


#include "fancontrolkcm.moc"