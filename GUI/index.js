const noble = require('@abandonware/noble')

const BLE_SERVICE_UUID = "e30a2f5c-0a65-4473-996b-4f37bcf84137"
const BLE_CHARACTERISTIC_UUID = "89a81d4a-b71f-48ce-b653-46d59180826b"

console.log('starting')

noble.on('stateChange', function (state) {
  console.log('state change')
  if (state === 'poweredOn') {
    console.log('on -> Start scanning')
    noble.startScanning( [], true );
  } else {
    console.log('off')
    noble.stopScanning();
  }
});

noble.on('discover', function (peripheral) {

  console.log(`peripheral discovered (${peripheral.id} with address <${peripheral.address}, ${peripheral.addressType}>, connectable ${peripheral.connectable}, RSSI ${peripheral.rssi}:`);
});
