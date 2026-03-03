using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Threading.Tasks;
using System.Net.Http;
using System.Net.Http.Headers;
using System.Runtime.InteropServices;
using System.Security.Policy;
using System.Text.Json;
using System.Diagnostics;
using System.Threading;
using System.Xml.Linq;

namespace RedfishLib
{

    public enum SESSION_STATUS
    {
        OK,
        NO_TOKEN,
        FAIL,
        TIMEOUT
    }
    class RedfishClient : IDisposable
    {
        private readonly HttpClient _client;
        private readonly string _baseUrl;
        private readonly string _authUrl;
        private readonly string _user;
        private readonly string _passwd;
        private string _authToken;  // X-Auth-Token 저장

        public RedfishClient(string baseUrl, string username, string password, string authUrl)
        {
            _baseUrl = baseUrl;
            _user = username;
            _passwd = password;
            _authUrl = authUrl;

            var handler = new HttpClientHandler
            {
                ServerCertificateCustomValidationCallback = (message, cert, chain, sslPolicyErrors) => true
            };

            _client = new HttpClient(handler)
            {
                BaseAddress = new Uri(baseUrl),
                Timeout = TimeSpan.FromSeconds(3) // 3초 타임아웃 설정
            };
            _authUrl = authUrl;

            //Task.Run(async () => await Login()).Wait();
        }

		public async Task<(SESSION_STATUS,string)> ResetAction()
		{
			// 리셋을 위한 고정된 경로
			var resetEndpoint = "/redfish/v1/Managers/bmc/Actions/Manager.Reset";
			//수정
            var jsonPayload = @"{""ResetType"": ""ForceRestart""}";
            //기존
			//var jsonPayload = "{\"ResetType\": \"ForceRestart\"}";

			// 이 요청에만 사용할 HttpRequestMessage 객체 생성
			using (var requestMessage = new HttpRequestMessage(HttpMethod.Post, resetEndpoint))
			{
				// 클래스 멤버 변수인 _user와 _passwd를 사용하여 기본 인증 헤더 설정
				var authHeaderValue = Encoding.ASCII.GetBytes($"{_user}:{_passwd}");
				requestMessage.Headers.Authorization = new AuthenticationHeaderValue("Basic", Convert.ToBase64String(authHeaderValue));
				requestMessage.Content = new StringContent(jsonPayload, Encoding.UTF8, "application/json");

				try
				{
					//확인 Log//
					// 1. BaseAddress와 RequestUri를 조합하여 완전한 URL을 만듭니다.
					var fullUri = new Uri(_client.BaseAddress, requestMessage.RequestUri);

					Console.WriteLine("\n---[ Request Message Bebug ]---");
					// 2. 조합된 완전한 URL을 출력합니다.
					Console.WriteLine($"Full URI: {fullUri}");
					Console.WriteLine($"Method: {requestMessage.Method}");
					Console.WriteLine("Headers:");

					// 3. 요청에 포함된 모든 헤더를 하나씩 출력합니다.
					// 기본 헤더 (e.g., Authorization)
					foreach (var header in requestMessage.Headers)
					{
						Console.WriteLine($"- {header.Key}: {string.Join(", ", header.Value)}");
					}
					// 콘텐츠 헤더 (e.g., Content-Type)
					foreach (var header in requestMessage.Content.Headers)
					{
						Console.WriteLine($"- {header.Key}: {string.Join(", ", header.Value)}");
					}

					// 4. 본문(Body) 내용을 확인하여 출력합니다.
					string bodyContent = await requestMessage.Content.ReadAsStringAsync();
					Console.WriteLine($"Body: {bodyContent}");
					Console.WriteLine("---[ End of Request Message ]---\n");
					//확인 Log//

					HttpResponseMessage response = await _client.SendAsync(requestMessage);
					string responseBody = await response.Content.ReadAsStringAsync();

                    

					if (response.IsSuccessStatusCode)
					{
						Console.WriteLine($"성공: {_baseUrl} BMC 리셋 명령 전송 완료.");
						return (SESSION_STATUS.OK, responseBody);
					}
					else
					{
						Console.WriteLine($"실패: 리셋 명령 전송 실패. 상태 코드: {response.StatusCode}");
						Console.WriteLine($"오류 내용: {responseBody}");
						return (SESSION_STATUS.FAIL, responseBody);
					}
				}
				catch (Exception ex)
				{
					Console.WriteLine($"치명적 오류: 리셋 요청 중 예외 발생. {ex.Message}");
					return (SESSION_STATUS.FAIL, "");
				}
			}
		}


		public async Task<SESSION_STATUS> Login()
        {
            var content = new StringContent(
                $"{{\"UserName\": \"{_user}\", \"Password\": \"{_passwd}\"}}",
                Encoding.UTF8,
                "application/json"
            );
            try
            {
                HttpResponseMessage response = await _client.PostAsync(_authUrl, content);

                if (response.IsSuccessStatusCode)
                {
                    // ✅ X-Auth-Token 저장
                    if (response.Headers.Contains("X-Auth-Token"))
                    {
                        _authToken = response.Headers.GetValues("X-Auth-Token").FirstOrDefault();
                        _client.DefaultRequestHeaders.Authorization = null; // Basic Auth 제거
                        _client.DefaultRequestHeaders.Add("X-Auth-Token", _authToken);
                        //Console.WriteLine($"Login Redfish 세션 로그인 성공! Token: {_authToken}");
                        return SESSION_STATUS.OK;
                    }

                    Console.WriteLine("Login Redfish 세션 로그인 실패! X-Auth-Token 헤더가 없습니다.");
                    return SESSION_STATUS.NO_TOKEN; 
                }
                else
                {
                    Console.WriteLine($"Login Redfish 세션 로그인 실패! Status: {response.StatusCode}");
                    return SESSION_STATUS.FAIL;
                }
            }
            catch (TaskCanceledException)
            {
                Console.WriteLine("Login 요청 시간 초과 (Timeout).");
                return SESSION_STATUS.TIMEOUT;
            }catch (Exception ex)
            {
                Console.WriteLine($"Login Request Fail : {ex.Message}");
                return SESSION_STATUS.FAIL;
            }

        }

        public async Task<(SESSION_STATUS , string)> GetAsync(string endpoint)
        {
            try
            {
                HttpResponseMessage response = await _client.GetAsync(endpoint);
                response.EnsureSuccessStatusCode();
                return (SESSION_STATUS.OK,  await response.Content.ReadAsStringAsync());
            }
            catch (HttpRequestException ex)
            {                
                return (SESSION_STATUS.FAIL, $"GetAsync Network Error : {ex.Message}");
            }
            catch (TaskCanceledException)
            {
                return (SESSION_STATUS.TIMEOUT, "GetAsync Timeout.");
            }
            catch (Exception ex)
            {
                return (SESSION_STATUS.FAIL, $"GetAsync Request Fail : {ex.Message}");
            }
        }

        public async Task Logout()
        {
            if (string.IsNullOrEmpty(_authToken))
                return;

            // ✅ 세션 로그아웃 요청
            await _client.DeleteAsync(_authUrl);
            //Console.WriteLine("Redfish Session Logout Compleate");
        }

        public void Dispose()
        {
            Task.Run(async () => await Logout()).Wait();
            _client.Dispose();
        }
    }

    public class RestBMC
    {
        public string IP = "192.168.50.144";        
        public string User = "gsi7";
        public string Password = "gsipw1234";
        public string Path = "/redfish/v1/Chassis/AVC_Baseboard/Sensors/temperature_DIMM_";
        public string AuthPath = "/redfish/v1/SessionService/Sessions";
        public string ResetPath = "/redfish/v1/Managers/bmc/Actions/Manager.Reset";
        RedfishClient client;

        int DimmCount = 24;
        int SensorCount = 48;
        public List<string> SubPath { get; set; }        
        public List<double> TempList { get; set; }

        public Thread GetThread;
        public bool IsRun = false;
        public bool IsRequestStop = true;

        public RestBMC()
        {
            
        }

        public int getDimmTempSize()
        {
            return TempList.Count;
        }

        public double getDimmTemp(int index)
        {
            if (index >= TempList.Count) return 0;
            return TempList[index];
        }

        public bool checkRun()
        {
            return IsRun;
        }   

        public void Start()
        {
            if (GetThread != null)
                if (GetThread.IsAlive) return;            
            IsRequestStop = false;
            GetThread = new Thread(doGetWork);
            GetThread.Start();
        }

        public void Stop()
        {
            IsRequestStop = true;
            if (GetThread == null) return;
            if (GetThread.IsAlive)
                GetThread.Join();
        }


        public async void doGetWork()
        {
            try
            {
                IsRun = true;
                while (!IsRequestStop)
                {
                    Stopwatch stopwatch = Stopwatch.StartNew();
                    SESSION_STATUS stat = await open();
                    if (stat != SESSION_STATUS.OK) // Not Connected
                    {
                        Thread.Sleep(5000);
                        for (int i = 0; i < TempList.Count; i++)
                            TempList[i] = 0;
                        continue;
                    }

                    

                    Task<double>[] tasks = SubPath.Select(path => getTemp(path)).ToArray();
                    double[] results = await Task.WhenAll(tasks);
                    
                    close();

                    for (int i = 0; i < results.Length; i++)                    
                        TempList[i] = results[i];

                    stopwatch.Stop();
                    long val = stopwatch.ElapsedMilliseconds;
                    int sleepTime = 1000 - (int)val;
                    if (sleepTime < 100)
                        sleepTime = 100;
                    //Console.WriteLine($"RestBMC Time: {val}ms delay: {sleepTime}ms");

                    Thread.Sleep(100);
                }
            }
            catch (Exception ex)
            {
                Console.WriteLine($"doGetWork() Exception: {ex.Message}");
            }
            finally
            {
                IsRun = false;
            }
        }

        

        public void initItems(int count)
        {
            DimmCount = count;
            SensorCount = DimmCount *2;
            
        }

        public void setList(List<string> setList)
        {
            SubPath = new List<string>();
            TempList = new List<double>();
            for (int i = 0; i < SensorCount; i++)
            {
                SubPath.Add(setList[i]);
                TempList.Add(0);
            }
        }

		public async Task<(SESSION_STATUS, string)> Reset()
		{
			try
			{
				client = new RedfishClient("https://" + IP, User, Password, ResetPath);
                var ret = await client.ResetAction();
				return (ret.Item1, ret.Item2);
			}
			catch (Exception ex)
			{
				return (SESSION_STATUS.FAIL,"");
			}
		}

		public async Task<SESSION_STATUS> open()
        {            
            try
            {
                client = new RedfishClient("https://" + IP, User, Password, AuthPath);
                var ret = await client.Login();
                return ret;
            }
            catch (Exception ex)
            {
                return SESSION_STATUS.FAIL;
            }

        }

        public void close()
        {
            try
            {
                client.Dispose();
            }
            catch (Exception ex)
            {
                Console.WriteLine($"close() Exception: {ex.Message}");
            }
        }

        

        public async Task<double> getTemp(string path)
        {
            try
            {
                var (state, response) = await client.GetAsync($"{path}");
                if (state != SESSION_STATUS.OK)
                {
                    //Console.WriteLine($"get() noPath:{path}  {response}");
                    return 0;
                }

                using (JsonDocument doc = JsonDocument.Parse(response))
                {
                    
                    if (doc.RootElement.TryGetProperty("Reading", out JsonElement readingElement) &&
                        readingElement.TryGetDouble(out double temperature))
                    {
                        return temperature;
                    }
                    return 0;
                }
            }
            catch (Exception ex)
            {
                //Console.WriteLine($"get() Exception: {ex.Message}");
                return 0;
            }
        }

    }



    public static class RestBMC_Static
    {
        public static string IP = "192.168.50.144";        
        public static string Path = "/redfish/v1/Chassis/AVC_Baseboard/Sensors/temperature_DIMM_";
        public static string AuthPath = "/redfish/v1/SessionService/Sessions";
        public static string User = "gsi7";
        public static string Password = "gsipw1234";
		public static string ResetPath = "/redfish/v1/Managers/bmc/Actions/Manager.Reset";


		static RedfishClient client;
        public static async Task<SESSION_STATUS> open(string ip, string user, string pwd)
        {
            IP = ip;
            User = user;
            Password = pwd;
            try
            {
                client = new RedfishClient("https://" + IP, User, Password, AuthPath);
                var ret = await client.Login();
                return ret;
            }
            catch (Exception ex)
            {
                return SESSION_STATUS.FAIL;
            }
            
        }
		public static async Task<(SESSION_STATUS, string)> Reset(string ip, string user, string pwd)
		{
			try
			{
				client = new RedfishClient("https://" + IP, User, Password, ResetPath);
				var ret = await client.ResetAction();
				return ret;
			}
			catch (Exception ex)
			{
				return (SESSION_STATUS.FAIL, "");
			}
		}

		public static void close()
        {
            try
            {
                client.Dispose();
            }
            catch (Exception ex)
            {
                Console.WriteLine($"close() Exception: {ex.Message}");
            }
        }

        public static async Task<double> getTemp(string select)
        {
            try
            {
                var (state, response) = await client.GetAsync($"{Path}{select}");
                if (state != SESSION_STATUS.OK)
                {
                    Console.WriteLine($"get() Error: {response}");
                    return 0;
                }

                //var jsonObj = JObject.Parse(response);
                //return jsonObj.Value<double?>("Reading") ?? 0.0;

                using (JsonDocument doc = JsonDocument.Parse(response))
                {
                    //Console.WriteLine("Full JSON Response:");
                    //Console.WriteLine(JsonSerializer.Serialize(doc.RootElement, new JsonSerializerOptions { WriteIndented = true }));

                    if (doc.RootElement.TryGetProperty("Reading", out JsonElement readingElement) &&
                        readingElement.TryGetDouble(out double temperature))
                    {
                        return temperature;
                    }
                    return 0;
                }
            }
            catch (Exception ex)
            {
                Console.WriteLine($"get() Exception: {ex.Message}");
                return 0;
            }
        }        

    }
}
