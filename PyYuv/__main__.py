import pygame
import time
import sys

from PyYuv import Yuv


def usage(name):
    print("Usage:")
    print("    %s yuvfilename width height (start_frame)" % name)

    sys.exit(-1)


def main():
    if len(sys.argv) < 4:
        usage(sys.argv[0])

    filename = sys.argv[1]
    width = int(sys.argv[2])
    height = int(sys.argv[3])

    _ = Yuv.open(filename, width, height)
    if _ is None:
        sys.exit(-1)

    screen = None

    pygame.init()

    frame_no = 0
    get_next = True

    if len(sys.argv) >= 5:
        frame_no = int(sys.argv[4])
        _.seek_frame(frame_no)

    width = -1
    height = -1

    while 1:
        if get_next:
            im = _.get_frame()
            image = pygame.image.frombuffer(im.tobytes(), im.size, "RGB")

            frame_no += 1
            get_next = False

        if im is None:
            break

        if im.size != (width, height):
            width, height = im.size

            pygame.display.set_mode((width, height))
            screen = pygame.display.get_surface()

        pygame.display.set_caption('frame: %d' % (frame_no))

        screen.blit(image, (0, 0))
        pygame.display.flip()

        for e in pygame.event.get():
            if e.type == pygame.QUIT:
                sys.exit(0)
            elif e.type == pygame.KEYDOWN:
                get_next = True

        time.sleep(1)


main()
