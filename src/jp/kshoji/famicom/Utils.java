package jp.kshoji.famicom;

import java.awt.image.BufferedImage;
import java.io.BufferedReader;
import java.io.File;
import java.io.FileInputStream;
import java.io.FileNotFoundException;
import java.io.IOException;
import java.io.InputStreamReader;
import java.util.ArrayList;
import java.util.List;

public class Utils {
	
	// 128ドット中何個一致していれば同じと見做すか
	private static final int sameRate = 128;
	
	/**
	 * キャラクタデータが一致しているかを判別する
	 * 
	 * @param src1
	 * @param src2
	 * @return
	 */
	public static boolean matches(byte[] src1, byte[] src2) {
		int matchCount = 0;
		for (int i = 0; i < src1.length; i++) {
			for (int bit = 0; bit < 8; bit++) {
				if ((src1[i] & (1 << bit)) == (src2[i] & (1 << bit))) {
					matchCount++;
				}
			}
		}
		return matchCount >= sameRate;
	}
	
	public static List<String> textfileToStringArray(File file) throws FileNotFoundException, IOException {
		BufferedReader reader = new BufferedReader(new InputStreamReader(new FileInputStream(file)), 65536);

		List<String> allLines = new ArrayList<String>();
		String line;
		while ((line = reader.readLine()) != null) {
			allLines.add(line);
		}
		reader.close();
		return allLines;
	}

	public static byte[] bufferedImageToChrData(BufferedImage src) {
		byte[] data = new byte[16];
		for (int y = 0; y < 8; y++) {
			for (int x = 0; x < 8; x++) {
				if ((src.getRGB(x, y) & 0xffffff) == 0x404040 ||
						(src.getRGB(x, y) & 0xffffff) == 0xc0c0c0
						) {
					data[y] |= 1 << (7 - x);
				}
			}
		}

		for (int y = 0; y < 8; y++) {
			for (int x = 0; x < 8; x++) {
				if ((src.getRGB(x, y) & 0xffffff) == 0x808080 ||
						(src.getRGB(x, y) & 0xffffff) == 0xc0c0c0
						) {
					data[8 + y] |= 1 << (7 - x);
				}
			}
		}
		return data;
	}
	
	private static void deleteDirectory(File f) {
		if (!f.exists()) {
			return;
		}

		if (f.isFile()) {
			f.delete();
		}

		if (f.isDirectory()) {
			File[] files = f.listFiles();
			for (int i = 0; i < files.length; i++) {
				deleteDirectory(files[i]);
			}
			f.delete();
		}
	}

	public static void cleanUpDirectory(String directoryName) {
		File directory = new File(directoryName);
		deleteDirectory(directory);
		directory.mkdir();
	}
}
