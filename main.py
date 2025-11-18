from physics import StellarObject
from physics import StellarSystem

sys = StellarSystem([StellarObject(4, 1410, (0,0), (-0.25, 0))], 0.0001)

objs = sys.get_objects()
# print(type(objs))
# print(len(objs))

# for obj in objs:
#     print(obj.get_mass())
#     print(obj.get_position())
#     print(obj.get_radius())

sys.add_object(StellarObject(10000, 1410, (1, 0), (0, 0)))

objs = sys.get_objects()
# print(len(objs))

# for obj in objs:
#     print(obj.get_mass())
#     print(obj.get_position())
#     print(obj.get_radius())

t = 0
dt = 0.0001