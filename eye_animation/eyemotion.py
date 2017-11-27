# **************************************************
# *                  Galen Helfter
# *              ghelfte@g.clemson.edu
# *                ghelfter@gmail.com
# *                  eyemotion.py
# **************************************************

import maya.cmds as cmds
import math
import sys

class EyeControl:
    def __init__(self, name='', ymin=0.0, ymax=1.0, yaxis='translateY',
                 yoffset=0.0, xmin=0.0, xmax=1.0, xaxis='translateX', xoffset=0.0):
        # Name of controller
        self.__name = name

        # Minimum values
        self.__x_min = xmin
        self.__y_min = ymin

        # Maximum values
        self.__x_max = xmax
        self.__y_max = ymax

        # Blink start values
        self.__x_blink = self.__x_max
        self.__y_blink = self.__y_max

        # Axis attribute names
        self.__x_axis = xaxis
        self.__y_axis = yaxis

        # Axis offsets
        self.__x_offset = xoffset
        self.__y_offset = yoffset

        # Axis inversions
        self.__inv_x = 1.0
        self.__inv_y = 1.0

        # User variables
        self.prev_x = 0.0
        self.prev_y = 0.0
        self.start_x = 0.0
        self.start_y = 0.0
        self.end_x = 0.0
        self.end_y = 0.0
        self.is_lower = False

    def get_name(self):
        return self.__name
    def get_min_x(self):
        return self.__x_min
    def get_min_y(self):
        return self.__y_min
    def get_max_x(self):
        return self.__x_max
    def get_max_y(self):
        return self.__y_max
    def get_x_axis(self):
        return self.__x_axis
    def get_y_axis(self):
        return self.__y_axis

    # Get positions
    def get_pos_x(self, time):
        return cmds.getAttr('%s.%s' % (self.__name, self.__x_axis), \
                            time=time) - self.__x_offset
    def get_pos_y(self, time):
        return cmds.getAttr('%s.%s' % (self.__name, self.__y_axis), \
                            time=time) - self.__y_offset

    def set_blink_x(self, value):
        self.__x_blink = value
    def set_blink_y(self, value):
        self.__y_blink = value

    def get_blink_x(self):
        return self.__x_blink
    def get_blink_y(self):
        return self.__y_blink

    # Get inversion of axis
    def get_invert_x(self):
        return self.__inv_x
    def get_invert_y(self):
        return self.__inv_y

    # Get normalized positions
    def get_x_norm(self, pos):
        return float((pos - self.__x_min) / (self.__x_max - self.__x_min))
    def get_y_norm(self, pos):
        return float((pos - self.__y_min) / (self.__y_max - self.__y_min))

    # Get blink normalized positions
    def get_x_norm_blink(self, pos):
        return float((pos - self.__x_min) / (self.__x_blink - self.__x_min))
    def get_y_norm_blink(self, pos):
        return float((pos - self.__y_min) / (self.__y_blink - self.__y_min))

    # Get positions based on normalized values
    def get_pos_norm_x(self, norm):
        return float((self.__x_max - self.__x_min)) * norm + self.__x_min

    def get_pos_norm_y(self, norm):
        return float((self.__y_max - self.__y_min)) * norm + self.__y_min

    # Get blink positions based on normalized values
    def get_x_pos_norm_b(self, norm):
        return float((self.__x_blink - self.__x_min)) * (1-norm) + self.__x_min
    def get_y_pos_norm_b(self, norm):
        return float((self.__y_blink - self.__y_min)) * (1-norm) + self.__y_min

    # Invert axes
    def invert_x(self):
        self.__inv_x = -self.__inv_x
    def invert_y(self):
        self.__inv_y = -self.__inv_y

    def translate_x(self, pos, time):
        cmds.setKeyframe(self.__name, time=time, value=pos, attribute=self.__x_axis)
    def translate_y(self, pos, time):
        cmds.setKeyframe(self.__name, time=time, value=pos, attribute=self.__y_axis)

# Setup and cleanup functions for units to make our lives easier
def setup_units():
    init_time_unit = cmds.currentUnit(query=True, time=True)
    init_len_unit = cmds.currentUnit(query=True, linear=True)
    cmds.currentUnit(time='sec')
    cmds.currentUnit(linear='cm')
    return (init_time_unit, init_len_unit)

def reset_units(units):
    cmds.currentUnit(time=units[0])
    cmds.currentUnit(linear=units[1])

# Here is the move function that has already been developed
# This function works differently, in that it takes a value between 0 and 100
# and maps that to the movement. Thus, it is not compatible with the model
# being used.
def current_move(t, start_y=0.0):
    a = 0.98
    b = 1.18
    result = 0
    mu = 37.0
    c = mu / 100

    if t <= mu:
        result = a - math.pow((t / mu), 2.0)
    else:
        exponent = -c * math.log(t - mu + 1)
        result = b - math.pow(math.e, exponent)+.02
    return result

# This function is a wrapper for current_move, taking it and mapping the
# passed in time to a value between 0 and 100. 
def current_move_wrap(t):
    # [0,  2] * 50 should map to [0, 100] new_time = t * 50
    new_time = t * 50
    return -current_move(new_time) + 0.98

def remap(value, old_min, old_max, new_min, new_max):
    return new_min + (value - old_min) * (new_max - new_min) / (old_max - old_min)

def blink2(func, ctrl, frame, frames, time):
    move_norm = func(2.0 * (float(frame) / frames))

    # Exploiting the fact that the controller will be between 0 and 1
    move_amount = 1.0 - ctrl.get_y_pos_norm_b(move_norm)
    print(move_amount)

    # If axis is inverted, we want to move the other direction
    if(ctrl.get_invert_y() == -1):
        current = ctrl.get_y_pos_norm_b(ctrl.get_pos_y(time))
        diff = move_amount - current
        move_amount = current - diff
        
    work_val = remap(move_amount, 1.0, 2.0, 0.0, 1.0)
    if(ctrl.is_lower):
        work_val = 1.0 - work_val
        remap(work_val, 0.0, 1.0, 0.0, 0.01)
        work_val /= 60.0
    else:
        work_val *= 0.03
        work_val -= 0.03
    move_amount = work_val        
    # Magic number for our lid, we want to remap from 0.0 to -0.3 for upper,
    # then 0.0 to 0.1 for lower

    ctrl.translate_y(move_amount, time)

    return move_norm

def blink_movement_list(lids, func, start_frame, end_frame, frames_sec):
    current_time = float(start_frame) / float(frames_sec)
    frame_diff = int(end_frame - start_frame)

    # Set up some initial information for use in the loop
    for lid in lids:
        lid.start_x = lid.get_pos_x(current_time)
        lid.start_y = lid.get_pos_y(current_time)
        lid.end_x = lid.get_pos_x(float(end_frame + 1) / float(frames_sec))
        lid.end_y = lid.get_pos_y(float(end_frame + 1) / float(frames_sec))

    # We are going to go at a higher resolutions than just one frame in
    # order to show more of the curve in the animation suite.
    for frame in range(0, 2*(frame_diff)):
        # Calculate the current time
        i = float(frame) / 2.0
        current_time = float(i + start_frame) / float(frames_sec)

        for lid in lids:
            lid.set_blink_y(lid.start_y)
            if float(i) / float(frame_diff) >= 0.37: # 0.37 is mu
                lid.set_blink_y(lid.end_y)
            blink2(func, lid, i, frame_diff, current_time)
            # Not sure if we can take advantage of a horizontal blink yet
            #if(lid.blink_x()):

# This is a more optimized version of the follow_eye function, in which
# a list of controllers can be passed in instead of only one controller
# at a time, should make maya process them faster as it won't need to
# reset the keyframe as often
def follow_eye_list(eye_ctrl, ctrls, start_frame, end_frame, frames_sec):
    cmds.currentTime(float(start_frame)/frames_sec, edit=True)
    for lid in ctrls:
        lid.prev_y = lid.get_pos_y(float(start_frame) / frames_sec)

    for frame in range(0, int(end_frame - start_frame)):
        # Calculate current time
        current_time = float(frame + start_frame) / float(frames_sec)

        # Normalize eye position and get eyelid target location
        norm = eye_ctrl.get_y_norm(eye_ctrl.get_pos_y(current_time))
        for ctrl in ctrls:
            work_norm = norm
            if(ctrl.get_invert_y() == -1):
                work_norm = (1-norm)
            target = ctrl.get_pos_norm_y(work_norm)
            ctrl.translate_y(target, current_time)

# Note: Must be run while the eyelid motion still darts off in order to detect
# the blinks in the movement.
def acquire_blinks(eye_ctrl, start_frame, end_frame, frames_sec, b_x, b_y):
    in_blink = False
    b_start = 0
    b_end = 0
    blinks = []

    for i in range(start_frame, end_frame):
        x = eye_ctrl.get_pos_x(float(i) / float(frames_sec))
        y = eye_ctrl.get_pos_y(float(i) / float(frames_sec))

        # See if it is a blink
        if y < b_y and x < b_x:
            # If we are not in a blink get initial information
            if not in_blink:
                in_blink = True
                b_start = i

        # If we were in a blink, we need to finish this one off
        elif in_blink:
            in_blink = False
            b_end = i
            blinks.append([b_start, b_end])

    # Post-processing, need to add a little time to blinks
    for blink in blinks:
        diff = blink[1] - blink[0]
        blink[1] += 3
        blink[0] -= 1
        if blink[0] < 0:
            blink[0] = 0
    return blinks    


# Main function, operates on all of the controllers we want to use
def main():
    frames_sec = 30.0
    units = setup_units()
    
    # Range for upper will be between 0.01 and -0.03
    r_u_lid = EyeControl('ctr_r_upperEyeLid_01', 0.0, 1.0, 'translateY', 1.0)
    l_u_lid = EyeControl('ctr_l_upperEyeLid_01', 0.0, 1.0, 'translateY', 1.0)
    r_l_lid = EyeControl('ctr_r_lowerEyeLid_01', 0.0, 1.0, 'translateY', 1.0) 
    l_l_lid = EyeControl('ctr_l_lowerEyeLid_01', 0.0, 1.0, 'translateY', 1.0)
    
    r_l_lid.is_lower = True
    l_l_lid.is_lower = True

    lids = [r_u_lid, l_u_lid, r_l_lid, l_l_lid]
    rlids = [r_u_lid, r_l_lid]
    llids = [l_u_lid, l_l_lid]

    # PUT BLINKS HERE
    # Put a list of [startframe, endframe]
    blinks = [[0, 10]]

    print(blinks)

    for blink in blinks:
        blink_movement_list(lids, current_move_wrap, blink[0], blink[1],
                            frames_sec)

    reset_units(units)

if __name__ == '__main__':
    main()
