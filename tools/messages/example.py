import messages

# quick message test
d = messages.simple_test()
print('d:', d.__dict__)
d.dummy = 1234
msg = d.pack()
print("msg len:", len(msg))

d_recv = messages.simple_test(msg)
#d_recv.unpack(msg)
print("received:", d_recv.dummy)

# gps v4 test
gps = messages.gps_v4()
gps.index = 0
gps.time_sec = 15.12345
gps.latitude_deg = 43.241
gps.longitude_deg = -93.520
gps.altitude_m = 278.5
gps.vn_ms = 1.5
gps.ve_ms = -2.7
gps.vd_ms = -0.02
gps.unixtime_sec = 1508247829.23
gps.satellites = 9
gps.horiz_accuracy_m = 1.7
gps.vert_accuracy_m = 2.8
gps.pdop = 4.2
gps.fix_type = 3
msg = gps.pack()
print("msg len:", len(msg))

# in steps (to reuse the instance)
gps_recv = messages.gps_v4()
gps_recv.unpack(msg)
print(gps_recv.__dict__)

# directly
gps_recv = messages.gps_v4(msg)
print(gps_recv.__dict__)