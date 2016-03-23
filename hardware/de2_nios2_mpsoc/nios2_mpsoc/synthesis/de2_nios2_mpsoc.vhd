-- Implements a simple Nios II system for the DE2 board.
-- Inputs:  SW7-0 are parallel port inputs to the Nios II system.
--          CLOCK_50 is the system clock.
--          KEY0 is the active-low system reset.
-- Outputs: LEDG7-0 are parallel port outputs from the Nios II system.
--          SDRAM ports correspond to the signals in Figure 2; their names are those
--          used in the DE2 User Manual.

LIBRARY ieee;
USE ieee.std_logic_1164.all;
USE ieee.std_logic_arith.all;
USE ieee.std_logic_unsigned.all;

ENTITY de2_nios2_mpsoc IS
PORT (
      SW : IN STD_LOGIC_VECTOR(17 DOWNTO 0);
      KEY : IN STD_LOGIC_VECTOR(3 DOWNTO 0);
      CLOCK_50 : IN STD_LOGIC;
      LEDG : OUT STD_LOGIC_VECTOR(7 DOWNTO 0);
		LEDR : OUT STD_LOGIC_VECTOR(17 DOWNTO 0);
		HEX0 : out std_logic_vector (6 downto 0);
	   HEX1 : out std_logic_vector (6 downto 0);
	   HEX2 : out std_logic_vector (6 downto 0);
	   HEX3 : out std_logic_vector (6 downto 0);
	   HEX4 : out std_logic_vector (6 downto 0);
	   HEX5 : out std_logic_vector (6 downto 0);
	   HEX6 : out std_logic_vector (6 downto 0);
	   HEX7 : out std_logic_vector (6 downto 0);
      DRAM_CLK, DRAM_CKE : OUT STD_LOGIC;
      DRAM_ADDR : OUT STD_LOGIC_VECTOR(11 DOWNTO 0);
      DRAM_BA_0, DRAM_BA_1 : BUFFER STD_LOGIC;
      DRAM_CS_N, DRAM_CAS_N, DRAM_RAS_N, DRAM_WE_N : OUT STD_LOGIC;
      DRAM_DQ : INOUT STD_LOGIC_VECTOR(15 DOWNTO 0);
      DRAM_UDQM, DRAM_LDQM : BUFFER STD_LOGIC;
--		GPIO_0 : inout std_logic_vector(35 downto 0) := (others => 'X'); -- export
--      GPIO_1 : inout std_logic_vector(35 downto 0) := (others => 'X'); -- export
--		LCD_ON	: out std_logic;
--	   LCD_BLON	: out std_logic;
--		lcd_RS   : out   std_logic;                                        -- RS
--      lcd_RW   : out   std_logic;                                        -- RW
--      lcd_data : inout std_logic_vector(7 downto 0)  := (others => 'X'); -- data
--      lcd_EN    : out   std_logic;                                         -- EN
      sram_DQ           : inout std_logic_vector(15 downto 0) := (others => 'X'); -- DQ
      sram_ADDR         : out   std_logic_vector(17 downto 0);                    -- ADDR
      sram_LB_N         : out   std_logic;                                        -- LB_N
      sram_UB_N         : out   std_logic;                                        -- UB_N
      sram_CE_N         : out   std_logic;                                        -- CE_N
      sram_OE_N         : out   std_logic;                                        -- OE_N
      sram_WE_N         : out   std_logic                                         -- WE_N
		);
END de2_nios2_mpsoc;

ARCHITECTURE Structure OF de2_nios2_mpsoc IS
COMPONENT nios2_mpsoc
PORT (
      clk_clk : IN STD_LOGIC;
      reset_reset_n : IN STD_LOGIC;
--      sdram_clk_clk : OUT STD_LOGIC;
      leds_green_export : OUT STD_LOGIC_VECTOR(7 DOWNTO 0);
      leds_red_export : OUT STD_LOGIC_VECTOR(17 DOWNTO 0);
      hex3_hex0_HEX0    : out   std_logic_vector(6 downto 0);                     -- HEX0
      hex3_hex0_HEX1    : out   std_logic_vector(6 downto 0);                     -- HEX1
      hex3_hex0_HEX2    : out   std_logic_vector(6 downto 0);                     -- HEX2
      hex3_hex0_HEX3    : out   std_logic_vector(6 downto 0);                     -- HEX3
      hex7_hex4_HEX4    : out   std_logic_vector(6 downto 0);                     -- HEX4
      hex7_hex4_HEX5    : out   std_logic_vector(6 downto 0);                     -- HEX5
      hex7_hex4_HEX6    : out   std_logic_vector(6 downto 0);                     -- HEX6
      hex7_hex4_HEX7    : out   std_logic_vector(6 downto 0);                     -- HEX7
      buttons_export    : in    std_logic_vector(3 downto 0)  := (others => 'X');  -- export
      switches_export : IN STD_LOGIC_VECTOR(17 DOWNTO 0);
      sdram_wire_addr : OUT STD_LOGIC_VECTOR(11 DOWNTO 0);
      sdram_wire_ba : BUFFER STD_LOGIC_VECTOR(1 DOWNTO 0);
      sdram_wire_cas_n : OUT STD_LOGIC;
      sdram_wire_cke : OUT STD_LOGIC;
      sdram_wire_cs_n : OUT STD_LOGIC;
      sdram_wire_dq : INOUT STD_LOGIC_VECTOR(15 DOWNTO 0);
      sdram_wire_dqm : BUFFER STD_LOGIC_VECTOR(1 DOWNTO 0);
      sdram_wire_ras_n : OUT STD_LOGIC;
		sdram_wire_we_n : OUT STD_LOGIC;
--      jp1_0_17_export   : inout std_logic_vector(17 downto 0) := (others => 'X'); -- export
--     jp1_18_35_export  : inout std_logic_vector(17 downto 0) := (others => 'X'); -- export
--      jp2_0_17_export   : inout std_logic_vector(17 downto 0) := (others => 'X'); -- export
--      jp2_18_35_export  : inout std_logic_vector(17 downto 0) := (others => 'X'); -- export
 --     lcd_ON		: out std_logic;
 --     lcd_BLON		: out std_logic;
 --     LCD_DATA          : INOUT STD_LOGIC_VECTOR (7 DOWNTO 0);
--      lcd_RS            : out   std_logic;                                        -- RS
 --     lcd_RW            : out   std_logic;                                        -- RW
 --     lcd_EN            : out   std_logic;                                        -- E		
      sram_DQ           : inout std_logic_vector(15 downto 0) := (others => 'X'); -- DQ
      sram_ADDR         : out   std_logic_vector(17 downto 0);                    -- ADDR
      sram_LB_N         : out   std_logic;                                        -- LB_N
      sram_UB_N         : out   std_logic;                                        -- UB_N
      sram_CE_N         : out   std_logic;                                        -- CE_N
      sram_OE_N         : out   std_logic;                                        -- OE_N
      sram_WE_N         : out   std_logic                                         -- WE_N
		);
END COMPONENT;

COMPONENT sdram_pll
PORT ( inclk0 : IN STD_LOGIC;
c0 : OUT STD_LOGIC );
END COMPONENT;

SIGNAL DQM : STD_LOGIC_VECTOR(1 DOWNTO 0);
SIGNAL BA : STD_LOGIC_VECTOR(1 DOWNTO 0);
SIGNAL ONE : STD_LOGIC;
BEGIN
  DRAM_BA_0 <= BA(0);
  DRAM_BA_1 <= BA(1);
  DRAM_UDQM <= DQM(1);
  DRAM_LDQM <= DQM(0);
  ONE <= '1';
-- Instantiate the Nios II system entity generated by the Qsys tool.
NiosII: nios2_mpsoc
PORT MAP (
          clk_clk => CLOCK_50,
          reset_reset_n => ONE,
--          sdram_clk_clk => DRAM_CLK,
          leds_green_export => LEDG,
			 leds_red_export => LEDR,
          switches_export => SW,
			 hex3_hex0_HEX0 => HEX0,
			 hex3_hex0_HEX1 => HEX1,
			 hex3_hex0_HEX2 => HEX2,
			 hex3_hex0_HEX3 => HEX3,
			 hex7_hex4_HEX4 => HEX4,
			 hex7_hex4_HEX5 => HEX5,
			 hex7_hex4_HEX6 => HEX6,
			 hex7_hex4_HEX7 => HEX7,
          buttons_export => KEY,
          sdram_wire_addr => DRAM_ADDR,
          sdram_wire_ba => BA,
          sdram_wire_cas_n => DRAM_CAS_N,
          sdram_wire_cke => DRAM_CKE,
          sdram_wire_cs_n => DRAM_CS_N,
          sdram_wire_dq => DRAM_DQ,
          sdram_wire_dqm => DQM,
          sdram_wire_ras_n => DRAM_RAS_N,
          sdram_wire_we_n => DRAM_WE_N, 
--          jp1_0_17_export   => GPIO_0(17 downto 0),   --   jp1_0_17.export
--          jp1_18_35_export  => GPIO_0(35 downto 18),  --  jp1_18_35.export
--          jp2_0_17_export   => GPIO_1(17 downto 0),   --   jp2_0_17.export
--          jp2_18_35_export  => GPIO_1(35 downto 18),  --  jp2_18_35.export
--          lcd_on            => lcd_on,
--          lcd_blon          => lcd_blon,
--          lcd_RS            => lcd_RS,            --        lcd.RS
--          lcd_RW            => lcd_RW,            --           .RW
--          lcd_data          => lcd_data,          --           .data
--          lcd_EN             => lcd_EN,             --           .E
          sram_DQ           => sram_DQ,           --       sram.DQ
          sram_ADDR         => sram_ADDR,         --           .ADDR
          sram_LB_N         => sram_LB_N,         --           .LB_N
          sram_UB_N         => sram_UB_N,         --           .UB_N
          sram_CE_N         => sram_CE_N,         --           .CE_N
          sram_OE_N         => sram_OE_N,         --           .OE_N
          sram_WE_N         => sram_WE_N          --           .WE_N
			 );

-- Instantiate the entity sdram_pll (inclk0, c0).
neg_3ns: sdram_pll PORT MAP (CLOCK_50, DRAM_CLK);

END Structure;
