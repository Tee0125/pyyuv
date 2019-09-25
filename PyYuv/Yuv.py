from PyYuv import _yuv
from PIL import Image

class Yuv(object):
    def __init__(self, filename, width, height):
        self.ctx = _yuv.open(filename, width, height)

    def get_frame(self):
        result = _yuv.get_frame(self.ctx)

        if result is None:
            return None

        frame, w, h, = result

        return Image.frombytes("RGB", (w, h), frame)


    def seek_frame(self, frame_no):
        return _yuv.seek_frame(self.ctx, frame_no);


    def __del__(self):
        if self.ctx is not None:
            _yuv.close(self.ctx)


def open(filename, width=352, height=288):
    """
    Opens yuv file
    """
    return Yuv(filename, width, height)
