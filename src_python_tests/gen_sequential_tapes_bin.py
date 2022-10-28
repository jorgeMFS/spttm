
def convert_to_bin_str(num, bits):
    return f"{num:0{bits}b}"[-bits:]


print("".join([convert_to_bin_str(i,6) for i in range(31)]))
