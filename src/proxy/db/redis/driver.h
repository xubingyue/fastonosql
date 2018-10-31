/*  Copyright (C) 2014-2018 FastoGT. All right reserved.

    This file is part of FastoNoSQL.

    FastoNoSQL is free software: you can redistribute it and/or modify
    it under the terms of the GNU General Public License as published by
    the Free Software Foundation, either version 3 of the License, or
    (at your option) any later version.

    FastoNoSQL is distributed in the hope that it will be useful,
    but WITHOUT ANY WARRANTY; without even the implied warranty of
    MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
    GNU General Public License for more details.

    You should have received a copy of the GNU General Public License
    along with FastoNoSQL.  If not, see <http://www.gnu.org/licenses/>.
*/

#pragma once

#include "proxy/driver/idriver_remote.h"  // for IDriverRemote

namespace fastonosql {
namespace core {
#if defined(PRO_VERSION)
class IModuleConnectionClient;
#endif
namespace redis {
class DBConnection;
}
}  // namespace core
}  // namespace fastonosql

namespace fastonosql {
namespace proxy {
namespace redis {

class Driver : public IDriverRemote {
  Q_OBJECT
 public:
  explicit Driver(IConnectionSettingsBaseSPtr settings);
  virtual ~Driver() override;

  virtual bool IsInterrupted() const override;
  virtual void SetInterrupted(bool interrupted) override;

  virtual core::translator_t GetTranslator() const override;

  virtual bool IsConnected() const override;
  virtual bool IsAuthenticated() const override;

#if defined(PRO_VERSION)
 Q_SIGNALS:
  void ModuleLoaded(core::ModuleInfo module);
  void ModuleUnLoaded(core::ModuleInfo module);
#endif

 private:
  virtual void InitImpl() override;
  virtual void ClearImpl() override;

  virtual core::FastoObjectCommandIPtr CreateCommand(core::FastoObject* parent,
                                                     const core::command_buffer_t& input,
                                                     core::CmdLoggingType logging_type) override;

  virtual core::FastoObjectCommandIPtr CreateCommandFast(const core::command_buffer_t& input,
                                                         core::CmdLoggingType logging_type) override;

  virtual core::IDataBaseInfoSPtr CreateDatabaseInfo(const core::db_name_t& name,
                                                     bool is_default,
                                                     size_t size) override;

  virtual common::Error SyncConnect() override WARN_UNUSED_RESULT;
  virtual common::Error SyncDisconnect() override WARN_UNUSED_RESULT;

  virtual common::Error ExecuteImpl(const core::command_buffer_t& command,
                                    core::FastoObject* out) override WARN_UNUSED_RESULT;
  virtual common::Error DBkcountImpl(core::keys_limit_t* size) override WARN_UNUSED_RESULT;

  virtual common::Error GetCurrentServerInfo(core::IServerInfo** info) override;
  virtual common::Error GetServerCommands(std::vector<const core::CommandInfo*>* commands) override;
#if defined(PRO_VERSION)
  virtual common::Error GetServerLoadedModules(std::vector<core::ModuleInfo>* modules);
#endif
  virtual common::Error GetCurrentDataBaseInfo(core::IDataBaseInfo** info) override;

  virtual void HandleDiscoveryInfoEvent(events::DiscoveryInfoRequestEvent* ev) override;
  virtual void HandleLoadServerPropertyEvent(events::ServerPropertyInfoRequestEvent* ev) override;
  virtual void HandleServerPropertyChangeEvent(events::ChangeServerPropertyInfoRequestEvent* ev) override;
  virtual void HandleLoadServerChannelsRequestEvent(events::LoadServerChannelsRequestEvent* ev) override;
  virtual void HandleBackupEvent(events::BackupRequestEvent* ev) override;
  virtual void HandleRestoreEvent(events::RestoreRequestEvent* ev) override;

  virtual void HandleLoadDatabaseContentEvent(events::LoadDatabaseContentRequestEvent* ev) override;

  virtual core::IServerInfoSPtr MakeServerInfoFromString(const std::string& val) override;

#if defined(PRO_VERSION)
  core::IModuleConnectionClient* proxy_;
#endif
  core::redis::DBConnection* impl_;
};

}  // namespace redis
}  // namespace proxy
}  // namespace fastonosql
