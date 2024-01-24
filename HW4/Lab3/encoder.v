//Verilog HDL for "ADC", "encoder" "functional"


module encoder (in, out);
    input      [15:0] in;
    output reg [3:0]  out;

always @* begin
       case(1)
        in[15]:  out = 4'd15;
        in[14]:  out = 4'd14;
        in[13]:  out = 4'd13;
        in[12]:  out = 4'd12;
        in[11]:  out = 4'd11;
        in[10]:  out = 4'd10;
        in[9]:   out = 4'd9;
        in[8]:   out = 4'd8;
        in[7]:   out = 4'd7;
        in[6]:   out = 4'd6;
        in[5]:   out = 4'd5;
        in[4]:   out = 4'd4;
        in[3]:   out = 4'd3;
        in[2]:   out = 4'd2;
        in[1]:   out = 4'd1;
        default: out = 4'd0;
    endcase 
end

endmodule
