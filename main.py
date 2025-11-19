from physics import StellarObject, StellarSystem
from pygame_helpers.draw import draw_object, draw_temp_obj_w_vec, draw_sidebar
from pygame_helpers.physical_quantities import screen_to_world, radius_calc

import pygame

pygame.init()
pygame.font.init()

FPS = 60
WIDTH, HEIGHT = 1440, 900
SIDEBAR_FRAC = 0.1

# scale of pixels -> AU (1 AU = 645 pixels)
# the sun would have a diameter of ~6 pixels (sun_r = 0.00465 AU, sun_d = 0.0093 AU)
# (6 px / 0.0093 AU) = 645.16 px/AU
SCALE = 645.

DT = 0.0001
STEPS_PER_FRAME = int(1/FPS/DT)

screen = pygame.display.set_mode((WIDTH, HEIGHT))

SIM_WIDTH = int(WIDTH * (1 - SIDEBAR_FRAC))
sim_surf = pygame.Surface((SIM_WIDTH, HEIGHT))
sidebar_surf = pygame.Surface((WIDTH - SIM_WIDTH, HEIGHT))

pygame.display.set_caption("Stellar System Sim")
clock = pygame.time.Clock()

stel_sys = StellarSystem(dt=DT)
sys_objs = stel_sys.get_objects()
running_sim = False

creating_obj = False
init_pos = None
curr_obj = {"name": "Sun (G)", "mass": 1, "density": 1410, "radius": radius_calc(1, 1410), "color": (255, 237, 227)}

def main():
    global running_sim
    global creating_obj
    global init_pos
    global curr_obj
    dt = 0.001
    running = True

    while running:
        clock.tick(FPS)

        for event in pygame.event.get():
            if event.type == pygame.QUIT:
                running = False
                continue

            if event.type == pygame.MOUSEBUTTONDOWN:
                mx, my = pygame.mouse.get_pos()

                if mx >= (SIM_WIDTH):
                    sidebar_mx = mx - SIM_WIDTH
                    for rect, obj in sidebar:
                        if rect.collidepoint((sidebar_mx, my)):
                            print(f"Selected: {obj["name"]}")
                            curr_obj = obj
                else:
                    creating_obj = True
                    init_pos = pygame.mouse.get_pos()
                
            if event.type == pygame.MOUSEBUTTONUP and creating_obj:
                end_pos = pygame.mouse.get_pos()

                drag_vec = [init_pos[0] -  end_pos[0], end_pos[1] - init_pos[1]]
                scaled_vec = [v*20/SCALE for v in drag_vec]

                init_pos = screen_to_world(sim_surf, init_pos, SCALE)
                mass, density, color = curr_obj["mass"], curr_obj["density"], curr_obj["color"]

                obj = StellarObject(mass, density, init_pos, scaled_vec, color)
                stel_sys.add_object(obj)
                
                velocity = sum([v**2 for v in scaled_vec])

                print(f"Created object: {curr_obj["name"]} at {init_pos} with velocity of {velocity} (AU/yr)")
                print(f"In m/s {velocity * 4743.72}")

                creating_obj = False
                init_pos = None

            if event.type == pygame.KEYDOWN:
                if event.key == pygame.K_SPACE:
                    running_sim = not running_sim
                
                if event.key == pygame.K_r:
                    stel_sys.reset()

                if event.key == pygame.K_q:
                    running = False
                    continue
        
        if running_sim:
            stel_sys.rk4_step()

        sim_surf.fill((0,0,0))
        sidebar = draw_sidebar(sidebar_surf, int(WIDTH*0.1), SCALE)

        
        for obj in sys_objs:
            draw_object(sim_surf, obj, SCALE)

        draw_temp_obj_w_vec(sim_surf, creating_obj, init_pos, curr_obj, SCALE)
        
        screen.blit(sim_surf, (0,0))
        screen.blit(sidebar_surf, (SIM_WIDTH, 0))
        pygame.display.flip()
        
    pygame.quit()


if __name__ == "__main__":
    main()