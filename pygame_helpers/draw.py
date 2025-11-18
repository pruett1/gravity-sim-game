import pygame
from physics import StellarObject
from pygame_helpers.physical_quantities import radius_calc

def draw_object(surface, obj, SCALE):
    x, y = obj.get_position()
    x *= SCALE
    y *= SCALE
    y = surface.get_height() - y
    radius = obj.get_radius() * SCALE
    color = obj.get_color()
    pygame.draw.circle(surface, color, (int(x), (int(y))), radius)

def draw_temp_obj_w_vec(surface, creating_obj, init_pos):
    if creating_obj and init_pos is not None:
        x1, y1 = init_pos
        x2, y2 = pygame.mouse.get_pos()

        arrow_end = (x1 - (x2 - x1), y1 - (y2 - y1))

        temp_obj = StellarObject(1, 1410, (0,0), (0, 0))

        pygame.draw.circle(surface, (255, 255, 255), (int(x1), int(y1)), temp_obj.get_radius())

        pygame.draw.line(surface, (169, 17, 83), (x1, y1), arrow_end, 2)

def render_label_fit(text, font_name, max_width, color=(255, 255, 255), max_size=16, min_size=6):
    size = max_size
    while size >= min_size:
        font = pygame.font.SysFont(font_name, size)
        label_surface = font.render(text, True, color)
        if label_surface.get_width() <= max_width:
            return label_surface
        size -= 1

    #use min font size if still doesnt fit
    font = pygame.font.SysFont(font_name, min_size)
    return font.render(text, True, color)

def draw_sidebar(surface, width, SCALE):
    MIN_PADDING = 10
    SURF_WIDTH = surface.get_width()
    FONT = "Arial"

    available_objects = [
        {"name": "Altinak Aa (O)", "mass": 33, "density": 5.816, "radius": radius_calc(33, 5.816), "color": (146, 181, 255)},
        {"name": "Regulus A (B)", "mass": 4.15, "density": 78.42, "radius": radius_calc(4.15, 78.42), "color": (162, 192, 255)},
        {"name": "Sirius A (A)", "mass": 2.063, "density": 578.7, "radius": radius_calc(2.063, 578.7), "color": (213, 224, 255)},
        {"name": "Iota Piscium (F)", "mass": 1.15, "density": 399.6, "radius": radius_calc(1.15, 399.6), "color": (249, 245, 255)},
        {"name": "Sun (G)", "mass": 1, "density": 1410, "radius": radius_calc(1, 1410), "color": (255, 237, 227)},
        {"name": "Alsafi (K)", "mass": 0.84, "density": 2574, "radius": radius_calc(0.84, 2574), "color": (255, 218, 181)},
        {"name": "Gliese 581 (M)", "mass": 0.295, "density": 15101, "radius": radius_calc(0.295, 15101), "color": (255, 181, 108)}
    ]

    item_hitboxes = []

    pygame.draw.rect(surface, (124, 124, 124), (SURF_WIDTH - width, 0, width, surface.get_height()))

    y_off = MIN_PADDING

    for i, obj in enumerate(available_objects):
        color = obj["color"]
        radius = obj["radius"]
        radius = min(radius * SCALE, width // 2 - 4) # ensure radii doesnt exceed the sidebar
        
        x = SURF_WIDTH - (width // 2)
        y = y_off + radius

        pygame.draw.circle(surface, color, (x, int(y)), int(radius))

        label_y = y + radius + 4

        name_label = render_label_fit(obj["name"], FONT, width - 2, (255, 255, 255), 32, 6)
        name_label_rect = name_label.get_rect(center=(x, label_y + (name_label.get_height() // 2)))
        surface.blit(name_label, name_label_rect)
        label_y += name_label.get_height() + 2

        mass_label = render_label_fit("Mass (M\u2906): " + str(obj["mass"]), FONT, width - 2, (255, 255, 255), 12, 6)
        mass_label_rect = mass_label.get_rect(center=(x, label_y + (mass_label.get_height() // 2)))
        surface.blit(mass_label, mass_label_rect)
        label_y += mass_label.get_height() + 2

        dens_label = render_label_fit("Density (kg/m\u00B3): " + str(obj["density"]), FONT, width - 2, (255, 255, 255), 12, 6)
        dens_label_rect = dens_label.get_rect(center=(x, label_y + (dens_label.get_height() // 2)))
        surface.blit(dens_label, dens_label_rect)
        label_y += (dens_label.get_height()) + 2

        item_height = label_y - y_off
        item_rect = pygame.Rect(SURF_WIDTH - width, y_off, width, item_height)

        y_off += item_height + MIN_PADDING

        item_hitboxes.append([item_rect, obj])

    return item_hitboxes