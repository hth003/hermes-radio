from xbee import XBee
import serial
# data form: "lat,long,alt,button"
PORT = 'COM10'
BAUD_RATE = 38400
ser = serial.Serial(PORT, BAUD_RATE)
#Create API object
xbee = XBee(ser)
database = []
bGPS = None # save the earlier gps value
def grabData(rawData):
    data = str(rawData)
    alert = None
    safe = None
    if "Alert" in data: #if button alert pressed
        alert = True
        return "alert"
    elif "Safe" in data: #if button safe pressed
        safe = True
        return "safe"
    else: #if this is gps data, get the nmea sentence (GGA)
        start = data.find('G')-1
        end = data.find('$',start+1)-4
        return data[start:end]

def parseGPS(GPSData): # get latitude, longtitude and altitude from nmea
    #($GPGGA,163851.622,,,,,0,00,,M,,M,,*76)
    if GPSData.find('G')==None or GPSData == '':
        return None
    data = GPSData.split(",")
    print(data)
    if data[2] != '' and data[4] !='' and data[9] !='' and data[9] != 'M':
        lat = float(data[2])
        lng = float(data[4])
        alt = float(data[9])
        return [lat,lng,alt]
    else:
        return None

def computation(packet,button):
    order = 0 # order 1 trigger LED and beeper
    isInDatabase = False
    proximity = 0.0002
    for i in range(len(database)): # check if the current location is in database within proximity
        if packet[0] - proximity < database[i][0] < packet[0] + proximity \
        and packet[1] - proximity < database[i][1] < packet[1] + proximity \
        and packet[2] - proximity < database[i][2] < packet[2] + proximity:
            isInDatabase = True
    if isInDatabase == True: # if in database, alert
        order = 1
        if button == "safe": # if buttonSafe is pressed, delete location from database
            del database[i]
    if button == "alert": # if button alert is pressed, add location 
        if isInDatabase == False:
            database.append(packet)
    return order

def main():
    print('Starting Program...')
    bGPS = None
    while True:
        try:
            response = xbee.wait_read_frame() #gather data from arduino
            print(response)
            processedData = grabData(response) # convert to button or gps nmea data
            print(processedData)
            if processedData == "alert": #if button press, use the gps data before it
                if bGPS is not None:
                    order = computation(bGPS,"alert")
            elif processedData == 'safe':
                if bGPS is not None :
                    order = computation(bGPS,"safe")
            else: # if gps data, process it and check if in database
                processedGPS = parseGPS(processedData)
                print(processedGPS)
                if processedGPS is not None:
                    bGPS=processedGPS
                    order = computation(bGPS,"none")
                else:
                    order=0
            print(order)
            print(database)
            if order == 1: # if order 1, alert at arduino
                ser.write('10101'.encode())
        except KeyboardInterrupt:
            break
    print('Closing...')
    ser.close()

    
