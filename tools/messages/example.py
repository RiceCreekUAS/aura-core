import messages

# quick message test
st = messages.simple_test()
print('st:', st.__dict__)
st.a = 1234
msg = st.pack()
print("msg len:", len(msg))

st_recv = messages.simple_test(msg)
#st_recv.unpack(msg)
print("received:", st_recv.a)

# gps v4 test
gps = messages.gps_v4()
gps.index = 0
gps.latitude_deg = 43.241
gps.longitude_deg = -93.520
gps.altitude_m = 278.5
gps.vn_ms = 1.5
gps.ve_ms = -2.7
gps.vd_ms = -0.02
gps.satellites = 9
msg = gps.pack()
print("msg len:", len(msg))

# in steps (to reuse the instance)
gps_recv = messages.gps_v4()
gps_recv.unpack(msg)
print(gps_recv.__dict__)

# directly
gps_recv = messages.gps_v4(msg)
print(gps_recv.__dict__)
