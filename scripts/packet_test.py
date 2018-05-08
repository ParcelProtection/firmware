import time
import serial
import threading
import struct
import datetime

def send_init_pkt():
  date = datetime.datetime.today()
  pkt = bytes([0x01, # init type
               34, # pkt_len 34
               0xEF, 0xBE, # package_id 0xBEEF
               0x00, 0x00, # reserved bytes
               0xE2, 0x07, # year 2018
               date.month,
               (date.weekday() + 1) % 7,
               date.day,
               date.hour,
               date.minute,
               date.second,
               0x8A, # carrier access code
               0xB2, # user access code
               0x03, # track drops and flips
               18]) # tracking len
  pkt += bytes(b'1ZA807T70336134832') # tracking number
  crc = pkt[0]
  for i in range(1,34):
    crc ^= pkt[i]
  pkt += bytes([crc]) # crc
  print("Sending init packet\n")
  ser.write(pkt)
  return

def send_dump_pkt():
  pkt = bytes([0x02, # dump type
               1, # pkt_len 1
               0x8A]) # access code
  crc = pkt[0]
  for i in range(1,3):
    crc ^= pkt[i]
  pkt += bytes([crc]) # crc
  print("Sending dump packet\n")
  ser.write(pkt)
  return

def send_status_pkt():
  pkt = bytes([0x00, # status type
               0x00, # pkt_len 0
               0x00]) # checksum 0
  print("Sending status packet\n")
  ser.write(pkt)
  return

def serial_read():
  ser.isOpen()
  print("Connected to parcel\n")
  print("Usage:")
  print("  's': status")
  print("  'i': initialize")
  print("  'd': get data\n")
  
  while running:
    #get packet header
    pkt_hdr = ser.read(2)
    pkt_type = pkt_hdr[0]
    crc = pkt_type
    payload_len = pkt_hdr[1]
    crc = crc ^ payload_len
    
    # main packet payload
    if pkt_type == 0x80: # ACK
      print("ACK:")
      
      # check CRC
      pkt_crc = ser.read(1)[0]
      if pkt_crc == crc:
        print("  CRC passed\n")
      else:
        print("  CRC failed - expected: {}, got: {}\n".format(pkt_crc, crc))
        
    elif pkt_type == 0x81: # status
      print("Status:")
      payload = ser.read(4)
      package_id = (payload[1] << 8) | payload[0]
      status_code = "uninitialized" if payload[2] == 0 else "tracking" if payload[2] == 1 else "error"
      for i in range(0,4):
        crc = crc ^ payload[i]
      
      # check CRC
      pkt_crc = ser.read(1)[0]
      print("  ID: 0x{:X}".format(package_id))
      print("  status: {}".format(status_code))
      if pkt_crc == crc:
        print("  CRC passed\n")
      else:
        print("  CRC failed - expected: {}, got: {}\n".format(pkt_crc, crc))
      
    elif pkt_type == 0x82: # dump
      print("Dump:")
      payload_hdr = ser.read(4)
      package_id = (payload_hdr[1] << 8) | payload_hdr[0]
      num_events = payload_hdr[2]
      for i in range(0,4):
        crc = crc ^ payload_hdr[i]
      print("  ID: 0x{:X}".format(package_id))
      print("  num_events: {}".format(num_events))
      
      # get events
      for i in range(0, num_events):
        event = ser.read(16)
        event_type = "drop" if event[0] == 0 else "flip"
        year = (event[5] << 8) | event[4]
        month = event[6]
        dow = event[7]
        day = event[8]
        hour = event[9]
        minute = event[10]
        second = event[11]
        data = (event[15] << 24) | (event[14] << 16) | (event[13] << 8) | event[12]
        print("  Event: {} {:02}/{:02}/{:02} {:02}:{:02}:{:02}".format(event_type, month, day, year, hour, minute, second))
        for j in range(0, 16):
          crc = crc ^ event[j]
      
      # check CRC
      pkt_crc = ser.read(1)[0]
      if pkt_crc == crc:
        print("  CRC passed\n")
      else:
        print("  CRC failed - expected: {}, got: {}\n".format(pkt_crc, crc))
      
    elif pkt_type == 0x8F: # NAK
      print("NAK:")
      
      # check CRC
      pkt_crc = struct.unpack('B', ser.read())[0]
      if pkt_crc == crc:
        print("  CRC passed\n")
      else:
        print("  CRC failed - expected: {}, got: {}\n".format(pkt_crc, crc))
        
    else:
      print("Unrecognized\n")
  
def user_input():
  global running
  while running:
    user_in = input()
    if user_in == "q":
      running = 0
    elif user_in == "i":
      send_init_pkt()
    elif user_in == "d":
      send_dump_pkt()
    elif user_in == "s":
      send_status_pkt()
      
  print("Closing")

# Main
running = 1
ser = serial.Serial('COM21', 9600)

thread_serial = threading.Thread(name='serial_read', target=serial_read)
thread_serial.daemon = True
thread_input = threading.Thread(name='user_input', target=user_input)

thread_serial.start()
thread_input.start()

thread_input.join()