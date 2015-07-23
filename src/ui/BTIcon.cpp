#include "BTIcon.h"

namespace MGUI
{

BTIcon::BTIcon(ilixi::Widget* parent)
        : SimpleIcon(parent),
          _bluetoothState(BluetoothSearching)
{
    setBluetoothState(BluetoothOff);
}

BTIcon::~BTIcon()
{
}

BluetoothState
BTIcon::getBluetoothState() const
{
    return _bluetoothState;
}

void
BTIcon::setBluetoothState(BluetoothState bluetoothState)
{
    if (_bluetoothState != bluetoothState)
        {
        _bluetoothState = bluetoothState;
            switch (_bluetoothState)
            {
                case BluetoothOff:
                    setImage("bluetooth_off");
                    break;
                case BluetoothSearching:
                    setImage("bluetooth_on");
                    break;
                case BluetoothConnected:
                    setImage("bluetooth_connected");
                    break;
                default:
                    break;
            }
        }
}

} /* namespace MGUI */
