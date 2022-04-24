from PIL import Image


def pnp_to_bmp_with_transparency(png_file, bmp_file):
    img = Image.open(png_file)
    img.save(bmp_file, 'BMP')


name = 'capam'
pnp_to_bmp_with_transparency(name + '.png', name + '.bmp')
