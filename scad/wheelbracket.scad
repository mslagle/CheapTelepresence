width_in_millimeter = 25;
length_in_millimeter = 80;
bracket_height_in_millimeter = 6.0;
screw_hole_diameter_in_millimeter = 5.3;


/*[hidden]*/
w = width_in_millimeter;
l = length_in_millimeter;
bracket_h = bracket_height_in_millimeter;
hole_d = screw_hole_diameter_in_millimeter;

 // Program Section //
//-----------------//

short_I_shape();

 // Module Section //
//----------------//


module short_I_shape() {
    color("YellowGreen") 
    {
        difference() 
        {
            linear_extrude(height = bracket_h)
            {
                polygon( points = [[0, 0], [0, l], [w, l], [w, 0]]);
            }
            
            translate([w/2, 10, 0])
                hole_shape(5);
            
            translate([w/2, 40, 0])
                hole_shape(5);
        }
        
        rotate ([90,0,0])
        {
            translate([0, -l, -l])
            {
                difference()
                {
                    linear_extrude(height = bracket_h)
                    {
                        polygon( points = [[-10, l], [-10, l+45], [w+10, l+45], [w+10, l]]);
                    }
                    
                   translate([w/2 - 16, l+15, 0])
                        hole_shape(3);
                    
                    translate([w/2 - 16, l+15+22, 0])
                        hole_shape(3);
                    
                    translate([w/2+16, l+15, 0])
                        hole_shape(3);
                    
                    translate([w/2+16, l+15+22, 0])
                        hole_shape(3);
                }
            }
        }
    }
    
    rotate ([0,-90,0])
    {
        translate([0, 0, -w/2 - bracket_h/2])
        {
            linear_extrude(height = bracket_h)
            {
                polygon( points = [[0, 50], [40, l], [0, l]]);
            }
        }
    }
}

module hole_shape(hole_d) {
    translate([0, 0, -1]) {
        cylinder(d = hole_d, h = bracket_h + 3);
    }
}
