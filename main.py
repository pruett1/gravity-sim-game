from physics import StellarObject, StellarSystem
import pygame as pygame

pygame.init()

FPS = 60
WIDTH, HEIGHT = 1280, 720

# scale of pixels -> AU (1 AU = 645 pixels)
# the sun would have a diameter of ~6 pixels (sun_r = 0.00465 AU, sun_d = 0.0093 AU)
# (6 px / 0.0093 AU) = 645.16 px/AU
SCALE = 645.

DT = 0.001
STEPS_PER_FRAME = int(1/FPS/DT)

screen = pygame.display.set_mode((WIDTH, HEIGHT))
# screen.fill("black")
pygame.display.set_caption("Stellar System Sim")
clock = pygame.time.Clock()

stel_sys = StellarSystem(dt=DT)
sys_objs = stel_sys.get_objects()
running_sim = False

def screen_to_world(pos):
    x, y = pos
    return [x / SCALE, (HEIGHT - y) / SCALE]

def draw_object(obj):
    x, y = obj.get_position()
    x *= SCALE
    y *= SCALE
    y = HEIGHT - y
    radius = obj.get_radius() * SCALE
    pygame.draw.circle(screen, (255,255,255), (int(x), (int(y))), radius)

def main():
    global running_sim
    print(STEPS_PER_FRAME)
    dt = 0.001
    running = True

    while running:
        clock.tick(FPS)

        for event in pygame.event.get():
            if event.type == pygame.QUIT:
                running = False
                continue

            if event.type == pygame.MOUSEBUTTONDOWN:
                pos = screen_to_world(pygame.mouse.get_pos())
                obj = StellarObject(1, 1410, pos, (0, 0))
                stel_sys.add_object(obj)

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

        screen.fill((0,0,0))
        for obj in sys_objs:
            draw_object(obj)
        
        pygame.display.flip()
        
    pygame.quit()


if __name__ == "__main__":
    main()