import binascii

mode = input("Enter mode: Paused or Auto: ")

#open template file for reading
reader = open("pcan_template.xmt", "r")
template = reader.read()


#open binary file for reading
reader = open("GPIO_InfiniteLedToggling.bin", "rb", buffering=0)

if mode == 'Paused':
    #open transmit file and copy template, crate file if it doesn't exist
    writer = open("host_commands_paused.xmt", "w+")
    writer.write(template)
    writer.close()
    #reopen PCAN transmit file for appending
    writer = open("host_commands_paused.xmt", "a")
    mode = " Paused "
    Cycle_init = 1000 #ms
    delay      = 10 #ms
else:
    #open transmit file and copy template, crate file if it doesn't exist
    writer = open("host_commands.xmt", "w+")
    writer.write(template)
    writer.close()
    #reopen PCAN transmit file for appending
    writer = open("host_commands.xmt", "a")
    mode = ''
    Cycle_init = input("Enter cyle init value (in ms): ")
    delay = input("Enter delay value (in ms): ")
    try:
        Cycle_init = int(Cycle_init)
        delay = int(delay)
    except:
        print("Invlid input, must be integer")
        quit()

Payload    = [0] * 8 #init Payload list of size 8
Identifier = 0x100
index      = 0
Cycle      = Cycle_init
can_msg    = []      #init can_msg list
frames_so_far = 0    #how many frames have been sent so far
frame_number  = 0

crc = [0] #cyclic check

bytes_so_far = 0x0

while True:
    byte = reader.read(1) #reads 1 byte from fileP
    if not byte:
        #eof
        break
    byte = binascii.hexlify(byte) #get data in hex format and convert to ascii
    crc[frame_number] += (int(str(byte, 'ascii'), 16))
    Payload[bytes_so_far] = str(byte, 'ascii') +"h " #eg: 08h, 04h etc
    bytes_so_far += 1

    if bytes_so_far == 8: #one full can data frame message
        bytes_so_far = 0  #reset counter
        #create data payload string
        Payload_str = ""
        #bytes are reversed in memory
        Payload_str =  Payload[7] + Payload[6] + Payload[5] + Payload[4]
        Payload_str += Payload[3] + Payload[2] + Payload[1] + Payload[0]
        #increase cycle time
        Cycle = Cycle + delay
        #Calculate crc (sum)
        #create can message (Identifier + Cyle time + DLC + Data payload)
        # +Message ID
        # |            +Cycle time in ms (0=manual)
        # |            |  +Data length
        # |            |  |  +Frame type
        # |            |  |  |  +Message data
        # |            |  |  |  |
        # 100h       1000 8  D 08h 04h 03h 01h 20h 02h 00h 00h Paused ;
        if frames_so_far < 31:
            can_msg += [hex(Identifier + index)[2:] + "h" + " "*8 + str(Cycle) + " "*2 + "8  D " + Payload_str + mode + ";" + "\r\n"]
            frames_so_far += 1
        else:
            #add the "end of frame" commentary
            can_msg += [hex(Identifier + index)[2:] + "h" + " "*8 + str(Cycle) + " "*2 + "8  D " + Payload_str + mode + ";" + " end of frame " + str(frame_number) + "\r\n"]
            # +Message ID
            # |            +Cycle time in ms (0=manual)
            # |            |  +Data length
            # |            |  |  +Frame type
            # |            |  |  |  +Message data
            # |            |  |  |  |
            #300h        10321  3  D 0h 14h 76h  Paused;
            can_msg += ['30' + str(frame_number) + "h" + " "*8 + str(Cycle + 1) + " "*2 + "3  D " + str(frame_number) + "h " + hex(crc[frame_number])[2:4] + "h " + hex(crc[frame_number])[4:] + "h " + ' Paused' + ";" + "\r\n"]
            frames_so_far += 1

        #set/reset
        index += 1
        Payload = [0]*8

    if frames_so_far == 32: #one full 256 data payload
        print("CRC", frame_number, "=", hex(crc[frame_number]))
        crc.append(0)
        frames_so_far = 0 #reset counter
        frame_number += 1
        Identifier    = 0x100
        Cycle         = Cycle_init + (frame_number * 1000)
        index         = 0

    # end of reading from file

#check to see if the frame is complete (32 data_frames)
#check to see if there is an incomplete data paylod (just 4 bytes)
if frames_so_far > 0:   #complete the frame with empty bytes (0xFF)
    #check if last paylod is complete
    if Payload[0] != 0:
        #comple last 4 bytes
        Payload[4] = 'ffh '
        Payload[5] = 'ffh '
        Payload[6] = 'ffh '
        Payload[7] = 'ffh '
        #calculate CRC
        crc[frame_number] += 0xff * 4
        #create data payload string
        Payload_str = ""
        #bytes are reversed in memory
        Payload_str =  Payload[7] + Payload[6] + Payload[5] + Payload[4]
        Payload_str += Payload[3] + Payload[2] + Payload[1] + Payload[0]
        #increase cycle time
        Cycle = Cycle + delay

        #create can message (Identifier + Cyle time + DLC + Data payload)
        if frames_so_far < 31:
            can_msg += [hex(Identifier + index)[2:] + "h" + " "*8 + str(Cycle) + " "*2 + "8  D " + Payload_str +  mode + ";" + "\r\n"]
            frames_so_far += 1
            index += 1
        else:
            can_msg += [hex(Identifier + index)[2:] + "h" + " "*8 + str(Cycle) + " "*2 + "8  D " + Payload_str +  mode + ";" + " end of frame " + str(frame_number) + "\r\n"]
            frames_so_far += 1

    while True: #complete the frame with 32 lines
        if frames_so_far < 31:
            #create empty lines: (ffh ffh ffh ffh ffh ffh ffh ffh)
            Payload_str = "ffh " * 8
            #calculate CRC
            crc[frame_number] += 0xff * 8
            #increase cycle time
            Cycle = Cycle + delay
            can_msg += [hex(Identifier + index)[2:] + "h" + " "*8 + str(Cycle) + " "*2 + "8  D " + Payload_str +  mode + ";" + "\r\n"]
            frames_so_far += 1
            index += 1
        else:
            Payload_str = "ffh " * 8
            #calculate CRC
            crc[frame_number] += 0xff * 8
            print("CRC", frame_number, "=", hex(crc[frame_number]))
            #increase cycle time
            Cycle = Cycle + delay
            can_msg += [hex(Identifier + index)[2:] + "h" + " "*8 + str(Cycle) + " "*2 + "8  D " + Payload_str +  mode + ";" + " end of frame " + str(frame_number) + "\r\n"]
            can_msg += ['30' + str(frame_number) + "h" + " "*8 + str(Cycle + 1) + " "*2 + "3  D " + str(frame_number) + "h " + hex(crc[frame_number])[2:4] + "h " + hex(crc[frame_number])[4:] + "h " + ' Paused' + ";" + "\r\n"]
            break

#jump to user app command
#090h           0  0  D ; jump to user app
can_msg += ["090h        "+ str(Cycle + 1000) + "  0  D ; jump to user app" + "\r\n"]

#print data in console (for verification)
for msg in can_msg:
    print(msg)

#write to file
writer.writelines(can_msg)

#close files
writer.close()
reader.close()
