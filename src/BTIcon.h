#ifndef BTICON_H_
#define BTICON_H_

#include "SimpleIcon.h"

namespace MGUI
{

class BTIcon : public SimpleIcon
{
public:
    BTIcon(ilixi::Widget* parent = NULL);

    virtual
    ~BTIcon();

    BluetoothState
    getBluetoothState() const;

    void
    setBluetoothState(BluetoothState bluetoothState);

private:
    BluetoothState _bluetoothState;
};

} /* namespace MGUI */

#endif /* BTICON_H_ */
