import pygame
import math

def screen_to_world(surface, pos, SCALE):
    x, y = pos
    return [x / SCALE, (surface.get_height() - y) / SCALE]

def radius_calc(mass, density):
    AU = 149597870700
    M_S = 1.988416e30
    density = (density * AU**3 / M_S)
    radius = math.cbrt(1/(density/mass))
    return radius