package Concurrentes.ClientGUI;

import java.awt.Color;
import java.awt.Dimension;
import java.awt.EventQueue;
import java.awt.Toolkit;
import java.awt.event.ActionEvent;
import java.awt.event.ActionListener;
import java.io.BufferedReader;
import java.io.InputStream;
import java.io.InputStreamReader;
import java.util.Vector;

import javax.swing.BorderFactory;
import javax.swing.GroupLayout;
import javax.swing.JButton;
import javax.swing.JComboBox;
import javax.swing.JFrame;
import javax.swing.JLabel;
import javax.swing.JPanel;
import javax.swing.JScrollPane;
import javax.swing.JTextArea;
import javax.swing.JTextField;
import javax.swing.SwingConstants;

public class ClientUI extends JFrame{
	
	private static final long serialVersionUID = 1L;

	public ClientUI()
	{
		Dimension dimention = Toolkit.getDefaultToolkit().getScreenSize();
		int windowBorder = 100;
		
		setBounds(windowBorder,windowBorder,dimention.width - 2 * windowBorder, dimention.height - 2 * windowBorder);
		setDefaultCloseOperation(JFrame.EXIT_ON_CLOSE);
		setTitle("InfoFutbol 2.0 - Administrador de datos");
		
		setResizable(false);
		setVisible(true);
		
		JPanel container = new JPanel();
		JScrollPane scroll = new JScrollPane(container);
		getContentPane().add(scroll);
		
		JLabel title = new JLabel("Client");
		container.add(title);
		
		JLabel searchTitle = new JLabel("Search patterns:");
		container.add(searchTitle);
		
		JLabel patternTitleName = new JLabel("Name:");
		patternTitleName.setHorizontalAlignment(SwingConstants.RIGHT);
		container.add(patternTitleName);
		
		final JTextField patternFieldName = new JTextField(40);
		container.add(patternFieldName);
		
		JLabel patternTitleAddress = new JLabel("Address:");
		patternTitleAddress.setHorizontalAlignment(SwingConstants.RIGHT);
		container.add(patternTitleAddress);

		final JTextField patternFieldAddress = new JTextField(40);
		container.add(patternFieldAddress);
		
		JLabel patternTitlePhone = new JLabel("Phone:");
		patternTitlePhone.setHorizontalAlignment(SwingConstants.RIGHT);
		container.add(patternTitlePhone);

		final JTextField patternFieldPhone = new JTextField(40);
		container.add(patternFieldPhone);
		
		JLabel requestTypeTitle = new JLabel("Request type:");
		container.add(requestTypeTitle);

		JLabel requestActionTitle = new JLabel("Request action:");
		container.add(requestActionTitle);
		
		
		Vector<RequestActionType> requestActions = new Vector<RequestActionType>();
		requestActions.add(RequestActionType.READ);
		requestActions.add(RequestActionType.CREATE);
		requestActions.add(RequestActionType.UPDATE);
		requestActions.add(RequestActionType.DELETE);
		final JComboBox<RequestActionType> requestActionCombo = new JComboBox<RequestActionType>(requestActions);
		container.add(requestActionCombo);
		
		JButton searchButton = new JButton("Request");
		container.add(searchButton);
		
		final JTextArea responseArea = new JTextArea(20,50);
		responseArea.setEditable(false);
		responseArea.setBorder(BorderFactory.createLineBorder(Color.black));
		container.add(responseArea);
		
		GroupLayout layout = new GroupLayout(container);
		container.setLayout(layout);

		layout.setAutoCreateGaps(true);
		layout.setAutoCreateContainerGaps(true);
		
		layout.setHorizontalGroup(
				layout.createParallelGroup(GroupLayout.Alignment.LEADING)//GroupLayout.Alignment.LEADING
					.addComponent(title)
					.addComponent(searchTitle)
					.addGroup(layout.createSequentialGroup()
						.addComponent(patternTitleName)//, 80, 80, 80)
						.addComponent(patternFieldName))
					.addGroup(layout.createSequentialGroup()
						.addComponent(patternTitleAddress)//, 80, 80, 80)
						.addComponent(patternFieldAddress))
					.addGroup(layout.createSequentialGroup()
						.addComponent(patternTitlePhone)//, 80, 80, 80)
						.addComponent(patternFieldPhone))
					.addGroup(layout.createSequentialGroup()
						.addComponent(requestActionTitle)
						.addComponent(requestActionCombo)//, 80, 80, 80)
						.addComponent(searchButton))
					.addComponent(responseArea)
				);
		
		layout.setVerticalGroup(
				layout.createSequentialGroup()//GroupLayout.Alignment.LEADING
					.addComponent(title)
					.addComponent(searchTitle)
					.addGroup(layout.createParallelGroup(GroupLayout.Alignment.LEADING)
						.addComponent(patternTitleName)
						.addComponent(patternFieldName))
					.addGroup(layout.createParallelGroup(GroupLayout.Alignment.LEADING)
						.addComponent(patternTitleAddress)
						.addComponent(patternFieldAddress))
					.addGroup(layout.createParallelGroup(GroupLayout.Alignment.LEADING)
						.addComponent(patternTitlePhone)
						.addComponent(patternFieldPhone))
					.addGroup(layout.createParallelGroup(GroupLayout.Alignment.LEADING)
						.addComponent(requestActionTitle)
						.addComponent(requestActionCombo)
						.addComponent(searchButton))
					.addComponent(responseArea)
				);
		
		layout.linkSize(SwingConstants.HORIZONTAL, patternTitleName, patternTitleAddress, patternTitlePhone, requestActionTitle);
		layout.linkSize(SwingConstants.VERTICAL, patternTitleName, patternFieldName);
		layout.linkSize(SwingConstants.VERTICAL, patternTitleAddress, patternFieldAddress);
		layout.linkSize(SwingConstants.VERTICAL, patternTitlePhone, patternFieldPhone);

		layout.linkSize(SwingConstants.VERTICAL, requestActionCombo, searchButton);
		
		searchButton.addActionListener(new ActionListener()
		{
			@Override
			public void actionPerformed(ActionEvent arg0)
			{
				try 
				{ 
					int actionId = ((RequestActionType)requestActionCombo.getSelectedItem()).getId();
					String namePattern = patternFieldName.getText().length() == 0 ? "-a" :
						patternFieldName.getText();
					String addressPattern = patternFieldAddress.getText().length() == 0 ? "-a" :
						patternFieldAddress.getText();
					String phonePattern = patternFieldPhone.getText().length() == 0 ? "-a" :
						patternFieldPhone.getText();
					
					/* directorio/ejecutable es el path del ejecutable y un nombre */ 
					String arguments[] = {"./admin", actionId + "", namePattern, addressPattern, phonePattern, null};
					Process p = Runtime.getRuntime().exec("./admin", arguments); 
					
					// Se obtiene el stream de salida del programa 
		            InputStream is = p.getInputStream(); 
		             
		            /* Se prepara un bufferedReader para poder leer la salida más comodamente. */ 
		            BufferedReader br = new BufferedReader (new InputStreamReader (is)); 
		             
		            // Se lee la primera linea 
		            String aux = br.readLine(); 
		             
		            // Mientras se haya leido alguna linea 
		            while (aux!=null) 
		            { 
		                // Se escribe la linea en pantalla 
		            	responseArea.append(aux);
		                 
		                // y se lee la siguiente. 
		                aux = br.readLine(); 
		            } 
				} 
				catch (Exception e) 
				{ 
					/* Se lanza una excepción si no se encuentra en ejecutable o el fichero no es ejecutable. */ 
				}
			}
		});
	}

	/**
	 * @param args
	 */
	public static void main(String[] args)
	{
		EventQueue.invokeLater(new Runnable()
		{
			public void run()
			{
				try
				{
					new ClientUI();
				}
				catch (Exception e)
				{
					e.printStackTrace();
				}
			}
		});
	}

}
