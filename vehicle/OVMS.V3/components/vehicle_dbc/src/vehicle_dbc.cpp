/*
;    Project:       Open Vehicle Monitor System
;    Date:          14th March 2017
;
;    Changes:
;    1.0  Initial release
;
;    (C) 2011       Michael Stegen / Stegen Electronics
;    (C) 2011-2017  Mark Webb-Johnson
;    (C) 2011        Sonny Chen @ EPRO/DX
;
; Permission is hereby granted, free of charge, to any person obtaining a copy
; of this software and associated documentation files (the "Software"), to deal
; in the Software without restriction, including without limitation the rights
; to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
; copies of the Software, and to permit persons to whom the Software is
; furnished to do so, subject to the following conditions:
;
; The above copyright notice and this permission notice shall be included in
; all copies or substantial portions of the Software.
;
; THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
; IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
; FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
; AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
; LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
; OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN
; THE SOFTWARE.
*/

#include "ovms_log.h"
static const char *TAG = "vehicle_dbc";

#include "vehicle_dbc.h"
#include "dbc_app.h"

OvmsVehicleDBC::OvmsVehicleDBC()
  {
  m_dbc_can1 = NULL;
  m_dbc_can2 = NULL;
  m_dbc_can3 = NULL;
  }

OvmsVehicleDBC::~OvmsVehicleDBC()
  {
  if (m_dbc_can1)
    {
    m_dbc_can1->UnlockFile();
    delete m_dbc_can1;
    m_dbc_can1 = NULL;
    }
  if (m_dbc_can2)
    {
    m_dbc_can2->UnlockFile();
    delete m_dbc_can2;
    m_dbc_can2 = NULL;
    }
  if (m_dbc_can3)
    {
    m_dbc_can3->UnlockFile();
    delete m_dbc_can3;
    m_dbc_can3 = NULL;
    }
  }

bool OvmsVehicleDBC::RegisterCanBusDBC(int bus, CAN_mode_t mode, const char* dbc)
  {
  dbcfile* newdbc = new dbcfile;
  if (!newdbc->LoadString(dbc, strlen(dbc)))
    {
    delete newdbc;
    return false;
    }
  switch (bus)
    {
    case 1:
      m_dbc_can1 = newdbc;
      break;
    case 2:
      m_dbc_can2 = newdbc;
      break;
    case 3:
      m_dbc_can3 = newdbc;
      break;
    default:
      ESP_LOGE(TAG,"RegisterCanBusDBC(%d) out of range",bus);
      delete newdbc;
      return false;
      break;
    }

  newdbc->LockFile();
  OvmsVehicle::RegisterCanBus(bus, mode, (CAN_speed_t)(newdbc->m_bittiming.GetBaudRate()/1000));
  return true;
  }

bool OvmsVehicleDBC::RegisterCanBusDBCLoaded(int bus, CAN_mode_t mode, const char* dbcloaded)
  {
  dbcfile* newdbc = MyDBC.Find(dbcloaded);
  if (newdbc==NULL) return false;
  switch (bus)
    {
    case 1:
      m_dbc_can1 = newdbc;
      break;
    case 2:
      m_dbc_can2 = newdbc;
      break;
    case 3:
      m_dbc_can3 = newdbc;
      break;
    default:
      ESP_LOGE(TAG,"RegisterCanBusDBCPath(%d) out of range",bus);
      return false;
      break;
    }

  newdbc->LockFile();
  OvmsVehicle::RegisterCanBus(bus, mode, (CAN_speed_t)(newdbc->m_bittiming.GetBaudRate()/1000));
  return true;
  }

void OvmsVehicleDBC::IncomingFrameCan1(CAN_frame_t* p_frame)
  {
  // This should be called from the IncomingFrameCan1 handler of the derived vehicle class
  if (m_dbc_can1==NULL) return;
  // TODO: Find the DBC message for the given ID, and decode the
  // relevant signals
  }

void OvmsVehicleDBC::IncomingFrameCan2(CAN_frame_t* p_frame)
  {
  // This should be called from the IncomingFrameCan1 handler of the derived vehicle class
  if (m_dbc_can2==NULL) return;
  // TODO: Find the DBC message for the given ID, and decode the
  // relevant signals
  }

void OvmsVehicleDBC::IncomingFrameCan3(CAN_frame_t* p_frame)
  {
  // This should be called from the IncomingFrameCan1 handler of the derived vehicle class
  if (m_dbc_can3==NULL) return;
  // TODO: Find the DBC message for the given ID, and decode the
  // relevant signals
  }

OvmsVehiclePureDBC::OvmsVehiclePureDBC()
  {
  ESP_LOGI(TAG, "Pure DBC vehicle module");

  std::string dbctype;

  dbctype = MyConfig.GetParamValue("vehicle", "dbc.can1");
  if (dbctype.length()>0)
    {
    ESP_LOGI(TAG,"Registering can bus #1 as DBC %s",dbctype.c_str());
    RegisterCanBusDBCLoaded(1, CAN_MODE_LISTEN, dbctype.c_str());
    }

  dbctype = MyConfig.GetParamValue("vehicle", "dbc.can2");
  if (dbctype.length()>0)
    {
    ESP_LOGI(TAG,"Registering can bus #2 as DBC %s",dbctype.c_str());
    RegisterCanBusDBCLoaded(2, CAN_MODE_LISTEN, dbctype.c_str());
    }

  dbctype = MyConfig.GetParamValue("vehicle", "dbc.can3");
  if (dbctype.length()>0)
    {
    ESP_LOGI(TAG,"Registering can bus #3 as DBC %s",dbctype.c_str());
    RegisterCanBusDBCLoaded(3, CAN_MODE_LISTEN, dbctype.c_str());
    }
  }

OvmsVehiclePureDBC::~OvmsVehiclePureDBC()
  {
  ESP_LOGI(TAG, "Shutdown Pure DBC vehicle module");
  }

void OvmsVehiclePureDBC::IncomingFrameCan1(CAN_frame_t* p_frame)
  {
  OvmsVehicleDBC::IncomingFrameCan1(p_frame);
  }

void OvmsVehiclePureDBC::IncomingFrameCan2(CAN_frame_t* p_frame)
  {
  OvmsVehicleDBC::IncomingFrameCan2(p_frame);
  }

void OvmsVehiclePureDBC::IncomingFrameCan3(CAN_frame_t* p_frame)
  {
  OvmsVehicleDBC::IncomingFrameCan3(p_frame);
  }

class OvmsVehiclePureDBCInit
  {
  public: OvmsVehiclePureDBCInit();
  } MyOvmsVehiclePureDBCInit  __attribute__ ((init_priority (9000)));

OvmsVehiclePureDBCInit::OvmsVehiclePureDBCInit()
  {
  ESP_LOGI(TAG, "Registering Vehicle: DBC based vehicle (9000)");

  MyVehicleFactory.RegisterVehicle<OvmsVehiclePureDBC>("DBC","DBC based vehicle");
  }
