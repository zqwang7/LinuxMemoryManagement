
import java.awt.*;
import java.awt.event.MouseAdapter;
import java.awt.event.MouseEvent;
import java.awt.event.MouseMotionListener;
import java.io.BufferedReader;
import java.io.File;
import java.io.FileInputStream;
import java.io.FileNotFoundException;
import java.io.IOException;
import java.io.InputStreamReader;
import java.util.ArrayList;
import java.util.HashMap;
import java.util.HashSet;
import java.util.Iterator;
import java.util.LinkedList;
import java.util.Map;
import java.util.Timer;
import java.util.TimerTask;

import javax.swing.BoxLayout;
import javax.swing.JButton;
import javax.swing.JFrame;
import javax.swing.JLabel;
import javax.swing.JPanel;
import javax.swing.JScrollPane;
import javax.swing.JTable;
import javax.swing.table.AbstractTableModel;

public class test extends JFrame
{
	public JPanel innerPane = new PaintPanel();
	public JScrollPane scroll = new JScrollPane(innerPane);
	public Container outPanel = getContentPane();
	public JPanel northpanel = new JPanel(new BorderLayout());
	public JLabel mouse_pos_label = new JLabel("Welcome");
	public JPanel southpanel =  new JPanel(new BorderLayout());
	
	public static HashMap<Integer,HashSet<Integer> > map;
	public static ArrayList<ProcessInfo> process;
	public static JTable process_table;
	
	public static String [] column_names = {"进程id","父进程id","进程名称","进程大小(MB)"};
	public static int number_of_rows = 0;
	
	public static Integer select_process = 0;
	public static class ProcessInfo
	{
		public Integer pid;
		public Integer pid_parent;
		public String name;
		public Double size;
		public ProcessInfo(Integer pid,Integer pid_parent,String name,Double size)
		{
			this.pid = pid;
			this.pid_parent = pid_parent;
			this.name = name;
			this.size = size;
		}
	}
	public test()
	{
		outPanel.setLayout(new BorderLayout());
		//添加上边的
		northpanel.add(mouse_pos_label);
		outPanel.add(mouse_pos_label,BorderLayout.NORTH);
		
		//中间的绘图
		innerPane.setPreferredSize(new Dimension(1024, 65536));
		innerPane.setBackground(Color.yellow);
		scroll.setViewportView(innerPane);
		scroll.setHorizontalScrollBarPolicy(JScrollPane.HORIZONTAL_SCROLLBAR_AS_NEEDED);
		scroll.setVerticalScrollBarPolicy(JScrollPane.VERTICAL_SCROLLBAR_AS_NEEDED);
		scroll.setSize(1024, 400);
		innerPane.setLayout(null);
		outPanel.add(scroll,BorderLayout.CENTER);
		innerPane.addMouseMotionListener(new MouseMotionListener()
		{
			public void mouseDragged(MouseEvent arg0)
			{
			}
			public void mouseMoved(MouseEvent arg0)
			{
				Point temp = arg0.getPoint();
				int x = temp.x;
				int y = temp.y;
				int number = y / 16 * 256 + x / 4 + 1;
				mouse_pos_label.setText("Phisical frame: 0x" + new Integer(number).toHexString(number).toUpperCase());
			}
		});
		//下面的表格
		process_table = new JTable(new myTableModel());
		//process_table.setColumnName(column_names);
		process_table.setAutoCreateRowSorter(true);
		process_table.addMouseListener(new MouseAdapter()
		{
			public void mouseClicked(java.awt.event.MouseEvent e)
			{ 
				int temp_row = process_table.getSelectedRow();
				if(process != null && process.get(temp_row) != null)
				{
					select_process = process.get(temp_row).pid;
					innerPane.repaint();
				}
			}
		});
		
		
		JScrollPane southScroll = new JScrollPane(process_table);
		southScroll.setPreferredSize(new Dimension(300, 400));
		outPanel.add(southScroll,BorderLayout.EAST);
		this.pack();
	//	this.setResizable(false);
		//setSize(1324,700);
		setVisible(true);
	}
	
	class myTableModel extends AbstractTableModel
	{
		public int getColumnCount() {
			// TODO Auto-generated method stub
			return 4;
			//return 0;
		}
		public int getRowCount()
		{
			// TODO Auto-generated method stub
			return number_of_rows;
		}
		public Object getValueAt(int x, int y)
		{

			//System.out.println("size is" + process.size());
			if(process.size() > x)
			{
				ProcessInfo temp = process.get(x);
				switch(y)
				{
					case 0:
						return temp.pid;
					case 1:
						return temp.pid_parent;
					case 2:
						return temp.name;
					case 3:
						return temp.size;
					default:
						return "ERROR";
				}
			}
			return "";
		}
		public String getColumnName(int column)
		{
			return column_names[column];
		}
	}
	class PaintPanel extends JPanel
	{
		public PaintPanel()
		{
			
		}
		public void paintComponent(Graphics g)
		{
			super.paintComponent(g);
			g.setColor(Color.BLACK);
			for(int i = 0;i < 65536;i += 16)
			{
				g.drawLine(0,i,1024,i);
			}
			g.setColor(Color.RED);
			if(map != null &&  map.get(select_process) != null)
			{
				System.out.println("current process : " + select_process);
				HashSet<Integer> temp = new HashSet<Integer>(map.get(select_process));
				for(Integer pos : temp)
				{
					int x = pos % 256 * 4;
					int y = pos / 256 * 16;
					g.fillRect(x, y, 4, 16);
				}
			}
		}
	}
	public static void main(String args[])
	{
		
		Timer timer = new Timer();
		timer.schedule(new TimerTask() {
			public void run()
			{
				//File data = new File("/root/seq/file.txt");
				File data = new File("/proc/mymemory");
			    //BufferedReader br = null;
			    if(data.exists())
			    {
			    	BufferedReader   br = null;
			    	Process proc;
					try {
						proc = Runtime.getRuntime().exec("cat /proc/mymemory");
						br=new   BufferedReader(new   InputStreamReader(proc.getInputStream()));  
					} catch (IOException e1) {
						// TODO Auto-generated catch block
						e1.printStackTrace();
					}  
//			    	try
//			    	{
//			    		br = new BufferedReader(new InputStreamReader(new FileInputStream("/proc/mymemory")));
//			    		//br = new BufferedReader(new InputStreamReader(new FileInputStream("/root/seq/file.txt")));
//			    	} 
//			    	catch (FileNotFoundException e)
//			    	{
//						e.printStackTrace();
//					}
			    	String line2 = "";
			    	String line = "";
			    	try 
			    	{
			    		
			    		while((line2 = br.readLine()) != null)
						{
			    			//System.out.println("line size" + line2.length());
			    			line += line2;
						}
			    		br.close();
			    			HashMap<Integer,HashSet<Integer> > map2 = new HashMap<Integer,HashSet<Integer> >();
			    			ArrayList<ProcessInfo> process2 = new ArrayList<ProcessInfo>();
							String[] infos = line.split("\\|");
							for(int i = 0;i < infos.length;i++)
							{
								String[] temp = infos[i].split("#");
								String head = temp[0];
								String frame = "";
								if(temp.length >= 2)
									frame = temp[1];
								temp = head.split(" ");
								Integer pid = new Integer(temp[0]);
								Integer pid_parent = new Integer(temp[1]);
								String pid_name = temp[2];
								
								temp = frame.split(",");
								HashSet<Integer> frames = new HashSet<Integer>();
								for(int j = 0;j < temp.length;j++)
								{
									if(temp[j].length() != 0)
										frames.add(new Integer(temp[j]));
								}
								process2.add(new ProcessInfo(pid,pid_parent,pid_name,4 * frames.size() / 1024.0));
								map2.put(pid,frames);
								map = new HashMap<Integer,HashSet<Integer> >(map2);
								process = new ArrayList<ProcessInfo>(process2);
							 }
							number_of_rows = infos.length;
							if(process_table != null)
								process_table.repaint();
					} 
			    	catch (NumberFormatException e)
					{
						e.printStackTrace();
					}
			    	catch (IOException e) 
					{
						e.printStackTrace();
					}
			   }
		       else
		       {
		    	   System.out.println("file don't exist");
		       }
			    
			}
       	}, 0, 3000);
		new test();
	}
} 