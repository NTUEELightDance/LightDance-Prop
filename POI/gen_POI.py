from PIL import Image
import numpy as np
import sys

# fp = "./test_POI.jpg"
fp = sys.argv[1]

len_grip = 18
LED_NUM = 72 + len_grip
resolution = 1


dtheta = resolution * np.pi / 180

Im = Image.open(fp)
Im_array = np.array(Im)


def turn_to_color_code(Im_array):
    new_arr = np.zeros([Im_array.shape[0], Im_array.shape[1]], dtype=np.uint32)
    for i in range(Im_array.shape[0]):
        for j in range(Im_array.shape[1]):
            new_arr[i, j] = (
                (int(Im_array[i, j, 0] / 4) << 8)
                + (int(Im_array[i, j, 1] / 4) << 16)
                + (int(Im_array[i, j, 2] / 4))
            )
    return new_arr


new_arr = turn_to_color_code(Im_array)
radius = int(min(new_arr.shape) / 2)
center = (new_arr.shape[0] // 2, new_arr.shape[1] // 2)


def gen_POI(arr):
    init = np.zeros([LED_NUM])

    for i in range(LED_NUM):
        init[i] = (radius / LED_NUM) * (i)

    table = []
    theta = 0

    while theta < 2 * np.pi:
        x = np.copy(init)
        y = np.copy(init)
        x = (init * np.cos(theta)).astype(int)
        y = (init * np.sin(theta)).astype(int)

        strip = []
        for i in range(LED_NUM):
            strip.append(arr[x[i] + center[0], y[i] + center[1]])
        theta += dtheta
        table.append(strip)

    return table


def gen_simulate(table):
    fig = np.zeros([LED_NUM * 2, LED_NUM * 2, 3])

    init = np.array(list(range(LED_NUM)))

    theta = 0
    count = 0

    while theta < 2 * np.pi:
        x = (init * np.cos(theta)).astype(int)
        y = (init * np.sin(theta)).astype(int)

        for i in range(len_grip, LED_NUM):
            fig[x[i] + LED_NUM, y[i] + LED_NUM, 0] = int((table[count][i] >> 8) & 255)
            fig[x[i] + LED_NUM, y[i] + LED_NUM, 1] = int((table[count][i] >> 16) & 255)
            fig[x[i] + LED_NUM, y[i] + LED_NUM, 2] = int(table[count][i] & 255)

            # if table[count][i] != 0:
            #     print(
            #         hex(table[count][i]),
            #         hex((table[count][i] >> 8) & 255),
            #         hex((table[count][i] >> 16) & 255),
            #         hex(table[count][i] & 255),
            #         fig[x + LED_NUM, y + LED_NUM, 0],
            #         fig[x + LED_NUM, y + LED_NUM, 1],
            #         fig[x + LED_NUM, y + LED_NUM, 2],
            #         type(fig),
            #         type(fig[x + LED_NUM, y + LED_NUM, 2]),
            #         fig[x + LED_NUM, y + LED_NUM, 2].shape,
            #     )

        count += 1
        theta += dtheta

    return fig.astype(np.uint8)


table = gen_POI(new_arr)
print("table size: {} * {}".format(len(table), len(table[0])))
print(table[0])
fig = gen_simulate(table)

f = open("fig.txt", "w")
for row in fig:
    s = ""
    for i in row:
        # print(i)
        s += "{}".format(hex((i[0] << 16) + (i[1] << 8) + i[2])).rjust(10)
    s += "\n"
    f.write(s)
f.close()

print(Im_array.shape, np.max(Im_array))
print(fig.shape, np.max(fig))
Fig = Image.fromarray(fig)
Fig.show()

f = open("output.txt", "w")
f.write("LEDs[{}][{}] = ".format(len(table), len(table[0]) - len_grip))
f.write("{\n")
for strip in table:
    s = "\t{"
    s += "{}".format(hex(int(strip[len_grip])))
    for i in strip[len_grip + 1 :]:
        s += " ,{}".format(hex(int(i)))
    s += "},\n"
    f.write(s)
f.write("}\n")
f.close()
