import binascii

# *********************** OPEN FILES FOR READING / WRITING  **********************************************
bin_file = input("Enter filename(.bin): ")
mode = input("Enter mode: Paused or Auto: ")

# open template file for reading
reader = open("pcan_template.xmt", "r")
template = reader.read()

# open binary file for reading
reader = open(bin_file, "rb", buffering=0)

if mode == 'Paused':
    # open transmit file and copy template, crate file if it doesn't exist
    writer = open("host_commands_paused.xmt", "w+")
    writer.write(template)
    writer.close()
    # reopen PCAN transmit file for appending
    writer = open("host_commands_paused.xmt", "a")
    mode = " Paused "
    Cycle_init = 1000  # ms
    delay = 10  # ms
    delay_between_frames = 20 # ms
else:
    # open transmit file and copy template, crate file if it doesn't exist
    writer = open("host_commands.xmt", "w+")
    writer.write(template)
    writer.close()
    # reopen PCAN transmit file for appending
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
    delay_between_frames = 20 #ms
Payload = [] # init Payload list
Identifier = 0x100
index = 0
Cycle = Cycle_init
can_msg = []  # init can_msg list
frames_so_far = 0  # how many frames have been sent so far
frame_number = 0

crc = [0]  # cyclic check

bytes_so_far = 0x0

# ******************************* START READ FROM FILE *********************************************************
while True:
    byte = reader.read(1)  # reads 1 byte from file
    if not byte:
        # eof
        break
    byte = binascii.hexlify(byte)  # get data in hex format and convert to ascii
    crc[frame_number] += (int(str(byte, 'ascii'), 16))
    Payload.append(str(byte, 'ascii') + "h ") # eg: 08h, 04h etc
    bytes_so_far += 1

    if bytes_so_far == 8:  # one full can data frame message
        bytes_so_far = 0  # reset counter
        # create data payload string
        Payload_str = ""
        # bytes are reversed in memory
        for i in range(8):
            Payload_str = Payload[i] + Payload_str
        # increase cycle time
        Cycle = Cycle + delay
        # ID
        ID = hex(Identifier + index)[2:]
        # create can message (Identifier + Cyle time + DLC + Data payload)
        # +Message ID
        # |            +Cycle time in ms (0=manual)
        # |            |  +Data length
        # |            |  |  +Frame type
        # |            |  |  |  +Message data
        # |            |  |  |  |
        # 100h       1000 8  D 08h 04h 03h 01h 20h 02h 00h 00h Paused ;
        if frames_so_far < 31:
            can_msg_str = '{}h        {}  8  D {} {} ; \n'.format(ID, Cycle, Payload_str, mode)
            can_msg.append(can_msg_str)
            frames_so_far += 1
        else:
            # add the "end of frame" commentary
            can_msg_str = '{}h        {}  8  D {} {} ; end of frame {}\n'.format(ID, Cycle, Payload_str, mode,
                                                                                 frame_number)
            can_msg.append(can_msg_str)

            # Visual CRC check after each end of frame:
            # +Message ID
            # |            +Cycle time in ms (0=manual)
            # |            |  +Data length
            # |            |  |  +Frame type
            # |            |  |  |  +Message data
            # |            |  |  |  |
            # 300h        10321  3  D 0h 14h 76h  Paused;
            crc_1 = hex(crc[frame_number])[2:4]
            crc_2 = hex(crc[frame_number])[4:]
            can_msg_str = '300h        {}  3  D {}h {}h {}h Paused;\n'.format(Cycle+1, frame_number, crc_1, crc_2)
            can_msg.append(can_msg_str)
            frames_so_far += 1

        # set/reset
        index += 1
        Payload = []

    if frames_so_far == 32:  # one full 256 bytes data payload
        print("CRC", frame_number, "=", hex(crc[frame_number]))
        crc.append(0)
        frames_so_far = 0  # reset counter
        frame_number += 1
        Identifier = 0x100
        Cycle = Cycle + delay_between_frames
        index = 0

# **************** END OF FILE READ ******************************************************************

# **************** COMPLETE THE DATA FRAME ***********************************************************
# check to see if the frame is complete (32 data_frames)
# check to see if there is an incomplete data payload (just 4 bytes)
if frames_so_far > 0:  # complete the frame with empty bytes (0xFF)
    # check if last paylod is complete
    if len(Payload) < 8:
        # complete last 4 bytes
        for _ in range(4):
            Payload.append('ffh ')
        # calculate CRC
        crc[frame_number] += 0xff * 4
        # create data payload string
        Payload_str = ""
        # bytes are reversed in memory
        for i in range(8):
            Payload_str = Payload[i] + Payload_str
        # increase cycle time
        Cycle = Cycle + delay
        # ID
        ID = hex(Identifier + index)[2:]
        # create can message (Identifier + Cyle time + DLC + Data payload)
        if frames_so_far < 31:
            can_msg_str = '{}h        {}  8  D {} {} ; \n'.format(ID, Cycle, Payload_str, mode)
            can_msg.append(can_msg_str)
            frames_so_far += 1
            index += 1
        else:
            can_msg_str = '{}h        {}  8  D {} {} ; end of frame {}\n'.format(ID, Cycle, Payload_str, mode,
                                                                                 frame_number)
            can_msg.append(can_msg_str)
            frames_so_far += 1

    while True:  # complete the frame with 32 lines
        # create empty lines: (ffh ffh ffh ffh ffh ffh ffh ffh)
        Payload_str = "ffh " * 8
        # increase cycle time
        Cycle = Cycle + delay
        # calculate CRC
        crc[frame_number] += 0xff * 8
        # ID
        ID = hex(Identifier + index)[2:]
        if frames_so_far < 31:
            can_msg_str = '{}h        {}  8  D {} {} ; \n'.format(ID, Cycle, Payload_str, mode)
            can_msg.append(can_msg_str)
            frames_so_far += 1
            index += 1
        else:
            print("CRC", frame_number, "=", hex(crc[frame_number]))
            can_msg_str = '{}h        {}  8  D {} {} ; end of frame {}\n'.format(ID, Cycle, Payload_str, mode,
                                                                                 frame_number)
            can_msg.append(can_msg_str)
            crc_1 = hex(crc[frame_number])[2:4]
            crc_2 = hex(crc[frame_number])[4:]
            can_msg_str = '300h        {}  3  D {}h {}h {}h Paused;\n'.format(Cycle+1, frame_number, crc_1, crc_2)
            can_msg.append(can_msg_str)
            break

# ************************ DATA COMPLETE *****************************************

# jump to user app command
# 090h           0  0  D ; jump to user app
can_msg_str = "090h        {}  0  D; jump to user app\n".format(Cycle+1000)
can_msg.append(can_msg_str)


# print data in console (for verification)
for msg in can_msg:
    print(msg)

# write to file
writer.writelines(can_msg)

# close files
writer.close()
reader.close()
